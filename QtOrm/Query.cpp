#include "Query.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QSharedPointer>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>

#include "ConfigurationMap.h"
#include "SimpleSqlBuilder.h"

namespace QtOrm {
namespace Sql {

using namespace Config;
Query::Query(QObject *parent) : QObject(parent) {
}

Query::Query(const Query &other) : Query(other.parent()) {
  operator=(other);
}

QSharedPointer<QObject> Query::getById(const QString &className, const QVariant &id) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(className);

  if (reestr->contains(classBase->getTable(), id.toString()))
    return reestr->value(classBase->getTable(), id.toString());

  auto list = getListObject(className, classBase->getIdProperty()->getName(), QString(), id);

  if (list.size() == 0) {
    return QSharedPointer<QObject>();
  }

  if (list.size() > 1)
    throw FindMoreThatOneRecordException(QString::fromUtf8("Found %1 records has id equal %2").arg(list.size()).arg(id.toString()));

  return list.takeFirst();
}

QList<QSharedPointer<QObject>> Query::getListObject(const QString &className, const QString &property, const QString &column, const QVariant &value) {
  GroupConditions group;
  if (!property.isEmpty() || !column.isEmpty()) {
    Condition filter(this);
    if(property.isEmpty())
    {
      filter.setColumn(column);
    }
    else
    {
      filter.setPropertyName(property);
    }
    filter.setOperation(Operation::Equal);
    filter.setValue(value);
    group.addCondition(filter);
  }

  return getListObject(className, group);
}

QList<QSharedPointer<QObject>> Query::getListObject(const QString &className, const GroupConditions &conditions) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(className);

  SimpleSqlBuilder sqlBuilder = buildSimpleSqlBuilder(classBase);
  sqlBuilder.setConditions(conditions);

  QSqlQuery query = sqlBuilder.selectQuery();
  executeQuery(query);

  return getList(query, *sqlBuilder.getQueryModel());
}

void Query::saveObject(QSharedPointer<QObject> &object) {
  startTransaction();
  saveObjectWoStartTransaction(object);
  commit();
}

void Query::insertObject(QSharedPointer<QObject> &object) {
  saveAllOneToOne(object);

  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);

  SimpleSqlBuilder sqlBuilder = buildSimpleSqlBuilder(classBase);
  sqlBuilder.setObject(object);

  QSqlQuery query = sqlBuilder.insertQuery();
  executeQuery(query);

  QVariant newId;
  if (database.driver()->hasFeature(QSqlDriver::LastInsertId) && database.driverName() != "QPSQL") {
    newId = query.lastInsertId();
  } else {
    query.first();
    newId = query.record().value(0);
  }
  objectSetProperty(object, classBase->getIdProperty()->getName(), newId);
  insertObjectIntoReestr(classBase, object, newId);

  saveAllOneToMany(object);
}

void Query::updateObject(QSharedPointer<QObject> &object) {
  saveAllOneToOne(object);

  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);

  SimpleSqlBuilder sqlBuilder = buildSimpleSqlBuilder(classBase);
  sqlBuilder.setObject(object);

  QSqlQuery query = sqlBuilder.updateQuery();
  executeQuery(query);

  saveAllOneToMany(object);
}

void Query::deleteObject(QSharedPointer<QObject> &object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);

  SimpleSqlBuilder sqlBuilder = buildSimpleSqlBuilder(classBase);
  sqlBuilder.setObject(object);

  QSqlQuery query = sqlBuilder.deleteQuery();
  executeQuery(query);

  removeObjectFromReestr(object);
}

void Query::refresh(QSharedPointer<QObject> &object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);

  GroupConditions group;

  QSharedPointer<PropertyMap> property = classBase->getIdProperty();
  QString idPropertyName = property->getName();
  QVariant idPropertyValue = object->property(idPropertyName.toStdString().data());
  group.addConditionEqual(idPropertyName, idPropertyValue);

  SimpleSqlBuilder sqlBuilder = buildSimpleSqlBuilder(classBase);
  sqlBuilder.setConditions(group);

  QSqlQuery query = sqlBuilder.selectQuery();
  executeQuery(query);

  query.next();
  QSqlRecord record = query.record();
  QString idColumn = getQueryColumn(sqlBuilder.getQueryModel()->getMainTableModel(), property);
  if(!record.value(idColumn).isNull()) {
    refreshObjectData(object, sqlBuilder.getQueryModel()->getMainTableModel(), record);
  }
}

void Query::saveOneField(QSharedPointer<QObject> &object, const QString &propertyName) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  QSharedPointer<OneToMany> oneToMany = classBase->findOneToManyByPropertyName(propertyName);
  if (oneToMany) {
    saveOneToMany(object, oneToMany);
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    if (oneToOne && isIdOneToOneDefault(object, oneToOne)) {
      saveOneToOne(object, oneToOne);
    }

    SimpleSqlBuilder sqlBuilder = buildSimpleSqlBuilder(classBase);
    sqlBuilder.setObject(object);

    QSqlQuery query = sqlBuilder.updateOneColumnQuery(propertyName);
    executeQuery(query);
  }
}

QSqlDatabase Query::getDatabase() const {
  return database;
}

void Query::setDatabase(const QSqlDatabase &value) {
  database = value;
}

void Query::executeQuery(QSqlQuery &query) {
  if(tryReopenDatabaseConnectionIfNeed()) {
    throw DatabaseConnectionClosedException("Database connection was closed");
  }

  QString executedQuery = getSqlTextWithBindParams(query);
  if (!query.exec()) {
    emit executedSql(executedQuery);
    QString errorMsg = QString("Query: %1 \nError: %2").arg(executedQuery).arg(query.lastError().text());
    throw SqlException(errorMsg);
  }
  emit executedSql(executedQuery);
}

QList<QSharedPointer<QObject>> Query::getList(QSqlQuery &query, const QueryModel &queryModel) {
  QList<QSharedPointer<QObject>> objects;
  QSharedPointer<ClassMapBase> classBase = queryModel.getClassBase();
  QString mainTableAlias = queryModel.getMainTableModel()->getAlias();
  while (query.next()) {
    QSharedPointer<QObject> newObject = getObject(query.record(), classBase, mainTableAlias, queryModel.getMainTableModel());
    objects.append(newObject);
  }

  return objects;
}

void Query::fillObject(QSharedPointer<QObject> &object, QSharedPointer<QueryTableModel> &queryTableModel, const QSqlRecord &record) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  auto properties = classBase->getProperties();
  for (auto prop : properties) {
    QString queryColumn = getQueryColumn(queryTableModel, prop);
    QVariant value = record.value(queryColumn);
    if (value.isValid())
      objectSetProperty(object, prop->getName(), value);
  }
}

void Query::fillOneToMany(const QList<QSharedPointer<OneToMany>> &relations, const QString &idProperty, QSharedPointer<QObject> object) {
  for (QSharedPointer<OneToMany> oneToMany : relations) {
    QString refClass = oneToMany->getRefClass();
    QString property = oneToMany->getProperty();
    QString column = oneToMany->getColumn();
    QVariant value = object->property(idProperty.toStdString().data());

    QList<QSharedPointer<QObject>> qobjectList = getListObject(refClass, QString(), column, value);

    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(oneToMany->getRefClass());
    QVariant var = refClassBase->getVariantByObjectList(qobjectList);
    objectSetProperty(object, property, var);
  }
}

void Query::fillOneToOne(QSharedPointer<QObject> object, QSharedPointer<QueryTableModel> queryTableModel, const QSqlRecord &record) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()) {
    QString property = oneToOne->getProperty();
    QString refClass = Mapping::ClassMapBase::getTypeNameOfProperty(object, property);

    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(refClass);


    QSharedPointer<QueryJoin> join = queryTableModel->findJoinByColumnName(oneToOne->getTableColumn());
    QString tableAlias = join->getQueryTableModel()->getAlias();
    QString idColumn = getQueryColumn(join->getQueryTableModel(), refClassBase->getIdProperty());
    if(!record.value(idColumn).isNull()) {
      QSharedPointer<QObject> newObject = getObject(record, refClassBase, tableAlias, join->getQueryTableModel());
      QVariant var = refClassBase->getVariantByObject(newObject);
      objectSetProperty(object, property, var);
    }
  }
}

QSharedPointer<QObject> Query::getObject(const QSqlRecord &record, const QSharedPointer<ClassMapBase> &classBase, const QString &tableAlias, QSharedPointer<QueryTableModel> queryTableModel) {
  QSharedPointer<QObject> object;

  if (reestrContainsObject(classBase, record, tableAlias)) {
    object = getObjectFromReestr(classBase, record, tableAlias);
  } else {
    object = createNewInstance(classBase);
    insertObjectIntoReestr(classBase, record, object, tableAlias);

    fillObject(object, queryTableModel, record);
    fillOneToOne(object, queryTableModel, record);
    fillOneToMany(classBase->getOneToManyRelations(), classBase->getIdProperty()->getName(), object);

    if(updater){
      updater->connectToAllProperties(object);
    }
  }

  return object;
}

void Query::objectSetProperty(QSharedPointer<QObject> object, const QString &propertyName, const QVariant &value) {
  if (!object->setProperty(propertyName.toStdString().data(), value)) {
    QString textError =
        QString::fromUtf8("Unable to place value in %1.%2").arg(object->metaObject()->className()).arg(propertyName);
    throw UnableToSetValueException(textError);
  }
}

QSharedPointer<QObject> Query::createNewInstance(QSharedPointer<ClassMapBase> classBase) {
  QSharedPointer<QObject> newObject = QSharedPointer<QObject>(classBase->getMetaObject().newInstance());
  if (!newObject) {
    throw InstanceNotCreatedException("Object instance was not created(Missing Q_INVOKABLE in constructor?)");
  }
  return newObject;
}

bool Query::reestrContainsObject(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, const QString &tableAlias) {
  QString tableName = classBase->getTable();
  QVariant idValue = getIdFromRecord(classBase, record, tableAlias);
  return reestr->contains(tableName, idValue.toString());
}

QSharedPointer<QObject> Query::getObjectFromReestr(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, const QString &tableAlias) {
  QString tableName = classBase->getTable();
  QVariant idValue = getIdFromRecord(classBase, record, tableAlias);
  return reestr->value(tableName, idValue.toString());
}

void Query::insertObjectIntoReestr(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, QSharedPointer<QObject> object,
                                   const QString &tableAlias) {
  QString idColumn;
  if (tableAlias.isEmpty()) {
    idColumn = classBase->getIdProperty()->getColumn();
  } else {
    idColumn = tableAlias + "_" + classBase->getIdProperty()->getColumn();
  }

  QVariant idValue = record.value(idColumn);

  insertObjectIntoReestr(classBase, object, idValue);
}

void Query::insertObjectIntoReestr(QSharedPointer<ClassMapBase> classBase, QSharedPointer<QObject> object, QVariant idValue) {
  QString tableName = classBase->getTable();

  reestr->insert(tableName, idValue.toString(), object);
}

void Query::removeObjectFromReestr(QSharedPointer<QObject> object) {
  reestr->remove(object);
}

QVariant Query::getIdFromRecord(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, const QString &tableAlias) {
  QString idColumn = tableAlias + "_" + classBase->getIdProperty()->getColumn();
  return record.value(idColumn);
}

void Query::refreshObjectData(QSharedPointer<QObject> object, QSharedPointer<QueryTableModel> queryTableModel, const QSqlRecord &record) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  fillObject(object, queryTableModel, record);

  refreshedObject.append(object);

  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()) {
    QString oneToOnePropertyTypeName = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(oneToOnePropertyTypeName);
    QVariant propertyValue = object->property(oneToOne->getProperty().toStdString().data());
    QSharedPointer<QObject> oneToOneObj = refClassBase->getObjectByVariant(propertyValue);
    if(!refreshedObject.contains(oneToOneObj)) {
      QSharedPointer<QueryJoin> join = queryTableModel->findJoinByColumnName(oneToOne->getTableColumn());
      refreshObjectData(oneToOneObj, join->getQueryTableModel(), record);
    }
  }

  QVariant idValue = object->property(classBase->getIdProperty()->getName().toStdString().data());

  for (QSharedPointer<OneToMany> oneToMany : classBase->getOneToManyRelations()) {
    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(oneToMany->getRefClass());
    QString refColumn = oneToMany->getColumn();
    QList<QSharedPointer<QObject>> newChildren = getListObject(refClassBase->getClassName(), QString(), refColumn, idValue);

    QVariant propertyValue = object->property(oneToMany->getProperty().toStdString().data());
    QList<QSharedPointer<QObject>> currentChildren = refClassBase->getObjectListByVariant(propertyValue);
    for(QSharedPointer<QObject> child : newChildren) {
      if(currentChildren.contains(child) && !refreshedObject.contains(child)) {
        refresh(child);
      }
    }

    QVariant newChildrenValue = refClassBase->getVariantByObjectList(newChildren);
    object->setProperty(oneToMany->getProperty().toStdString().data(), newChildrenValue);
  }
}

QString Query::getSqlTextWithBindParams(QSqlQuery &query) {
  QString text;
  text = query.lastQuery() + "\n";

  auto boundValues = query.boundValues();
  for (auto it = boundValues.begin(); it != boundValues.end(); ++it)
  {
    QString value;
    if(!it.value().isNull() || it.value().isValid())
    {
      value = it.value().toString();
    }
    else
    {
      value = "null";
    }
    text += QString("%1 = %2\n").arg(it.key()).arg(value);
  }

  return text;
}

void Query::saveAllOneToOne(QSharedPointer<QObject> object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);;
  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()) {
    if (oneToOne->getSaveCascade()) {
      saveOneToOne(object, oneToOne);
    }
  }
}

void Query::saveOneToOne(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne) {
  QString className = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
  QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(className);

  QVariant propertyVariant = object->property(oneToOne->getProperty().toStdString().data());
  QSharedPointer<QObject> propertyObject = refClassBase->getObjectByVariant(propertyVariant);

  if (propertyObject) {
    Query subQuery(*this);
    connect(&subQuery, &Query::executedSql, this, &Query::executedSql);
    subQuery.saveObjectWoStartTransaction(propertyObject);
  }
}

void Query::saveAllOneToMany(QSharedPointer<QObject> object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  for (QSharedPointer<OneToMany> oneToMany : classBase->getOneToManyRelations()) {
    if (oneToMany->getSaveCascade())
      saveOneToMany(object, oneToMany);
  }
}

void Query::saveOneToMany(QSharedPointer<QObject> object, QSharedPointer<OneToMany> oneToMany) {
  QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(oneToMany->getRefClass());
  QVariant val = object->property(oneToMany->getProperty().toStdString().data());
  QList<QSharedPointer<QObject>> lst = refClassBase->getObjectListByVariant(val);
  for (QSharedPointer<QObject> obj : lst) {
    Query subQuery(*this);
    connect(&subQuery, &Query::executedSql, this, &Query::executedSql);
    subQuery.saveObjectWoStartTransaction(obj);
  }
}

QSharedPointer<Reestr> Query::getReestr() const {
  return reestr;
}

void Query::setReestr(QSharedPointer<Reestr> value) {
  reestr = value;
}

Query &Query::operator=(const Query &other) {
  reestr = other.getReestr();
  database = other.getDatabase();
  queryCache = other.getQueryCache();

  return *this;
}

void Query::saveObjectWoStartTransaction(QSharedPointer<QObject> object) {
  if (isIdObjectDefault(object)) {
    insertObject(object);
   } else {
    updateObject(object);
  }
}

void Query::startTransaction() {
  if (database.driver()->hasFeature(QSqlDriver::Transactions)) {
    database.transaction();
  }
}

void Query::commit() {
  database.commit();
}

void Query::rollback() {
  database.rollback();
}

bool Query::isIdObjectDefault(QSharedPointer<QObject> object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  QVariant idVal = object->property(classBase->getIdProperty()->getName().toStdString().data());
  return idVal.value<ulong>() == 0;
}

bool Query::isIdOneToOneDefault(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne) {
  QString className = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
  QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(className);

  QVariant propertyVariant = object->property(oneToOne->getProperty().toStdString().data());
  QSharedPointer<QObject> propertyObject = refClassBase->getObjectByVariant(propertyVariant);

  return isIdObjectDefault(propertyObject);
}

QString Query::getQueryColumn(QSharedPointer<QueryTableModel> queryTableModel, QSharedPointer<PropertyMap> property){
  return QString("%1_%2").arg(queryTableModel->getAlias()).arg(property->getColumn());
}

bool Query::tryReopenDatabaseConnectionIfNeed() {
  return !database.isOpen() && !database.open();
}

SimpleSqlBuilder Query::buildSimpleSqlBuilder(QSharedPointer<ClassMapBase> &classBase) {
  SimpleSqlBuilder sqlBuilder;
  sqlBuilder.setDatabase(database);
  sqlBuilder.setQueryCache(queryCache);
  sqlBuilder.setClassBase(classBase);

  return sqlBuilder;
}

QSharedPointer<AutoUpdater> Query::getUpdater() const
{
  return updater;
}

void Query::setUpdater(const QSharedPointer<AutoUpdater> &value)
{
  updater = value;
}

QSharedPointer<QueryCache> Query::getQueryCache() const{
  return queryCache;
}

void Query::setQueryCache(QSharedPointer<QueryCache> value){
  queryCache = value;
}

void Query::clearRefreshedObject() {
  refreshedObject.clear();
}
}
}
