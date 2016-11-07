#include "Query.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>

#include "QDebug"

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

QObject *Query::getById(const QString &className, const QVariant &id) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(className);

  if (reestr->contains(classBase->getTable(), id.toString()))
    return reestr->value(classBase->getTable(), id.toString());

  auto list = getListObject(className, classBase->getIdProperty().getName(), id);

  if (list->size() == 0) {
    return nullptr;
  }

  if (list->size() > 1)
    throw Exception(ErrorCode::FindMoreThatOneRecord,
                    QString::fromUtf8("Found %1 records has id equal %2").arg(list->size()).arg(id.toString()));

  return list->takeFirst();
}

QList<QObject *> *Query::getListObject(const QString &className, const QString property, const QVariant value) {
  GroupConditions group;
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(className);
  if (!property.isEmpty()) {
    QString column = classBase->getProperty(property).getColumn();
    Condition filter(this);
    filter.setPropertyName(column);
    filter.setOperation(Operation::Equal);
    filter.setValue(value);
    group.addCondition(filter);
  }

  return getListObject(className, group);
}

QList<QObject *> *Query::getListObject(const QString &className, const GroupConditions &conditions) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(className);

  SimpleSqlBuilder sqlBuilder;
  sqlBuilder.setDatabase(database);
  sqlBuilder.setClassBase(classBase);
  sqlBuilder.setConditions(conditions);
  QSqlQuery query = sqlBuilder.selectQuery();

  executeQuery(query);

  return getList(query, *sqlBuilder.getQueryModel());
}

void Query::saveObject(QObject &object) {
  startTransaction();
  saveObjectWoStartTransaction(object);
  commit();
}

void Query::insertObject(QObject &object) {
  saveAllOneToOne(object);

  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());

  SimpleSqlBuilder sqlBuilder;
  sqlBuilder.setDatabase(database);
  sqlBuilder.setClassBase(classBase);
  sqlBuilder.setObject(&object);
  QSqlQuery query = sqlBuilder.insertQuery();

  executeQuery(query);

  QVariant newId;
  if (database.driver()->hasFeature(QSqlDriver::LastInsertId) && database.driverName() != "QPSQL") {
    newId = query.lastInsertId();
  } else {
    query.first();
    newId = query.record().value(0);
  }
  objectSetProperty(object, classBase->getIdProperty().getName(), newId);
  insertObjectIntoReestr(*classBase, &object, newId);

  saveAllOneToMany(object);
}

void Query::updateObject(QObject &object) {
  saveAllOneToOne(object);

  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());

  SimpleSqlBuilder sqlBuilder;
  sqlBuilder.setDatabase(database);
  sqlBuilder.setClassBase(classBase);
  sqlBuilder.setObject(&object);
  QSqlQuery query = sqlBuilder.updateQuery();

  executeQuery(query);

  saveAllOneToMany(object);
}

void Query::deleteObject(QObject &object) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());

  SimpleSqlBuilder sqlBuilder;
  sqlBuilder.setDatabase(database);
  sqlBuilder.setClassBase(classBase);
  sqlBuilder.setObject(&object);
  QSqlQuery query = sqlBuilder.deleteQuery();

  executeQuery(query);

  removeObjectFromReestr(&object);
}

void Query::refresh(QObject &object) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());

  GroupConditions group;
  QString idPropertyName = classBase->getIdProperty().getName();
  QVariant idPropertyValue = object.property(idPropertyName.toStdString().data());
  group.addConditionEqual(idPropertyName, idPropertyValue);

  SimpleSqlBuilder sqlBuilder;
  sqlBuilder.setDatabase(database);
  sqlBuilder.setClassBase(classBase);
  sqlBuilder.setConditions(group);

  QSqlQuery query = sqlBuilder.selectQuery();
  query.next();
  QSqlRecord record = query.record();

  refreshObjectData(object, sqlBuilder.getQueryModel()->getMainTableModel(), record);
}

void Query::saveOneField(QObject &object, const QString &propertyName) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  OneToMany *oneToMany = classBase->findOneToManyByPropertyName(propertyName);
  if (oneToMany) {
    saveOneToMany(object, oneToMany);
  } else {
    OneToOne *oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    if (oneToOne && isIdOneToOneDefault(object, oneToOne)) {
      saveOneToOne(object, oneToOne);
    }

    SimpleSqlBuilder sqlBuilder;
    sqlBuilder.setDatabase(database);
    sqlBuilder.setClassBase(classBase);
    sqlBuilder.setObject(&object);
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
  QString executedQuery = getSqlTextWithBindParams(query);
  if (!query.exec()) {
    emit executedSql(executedQuery);
    QString errorMsg = QString("Query: %1 \nError: %2").arg(executedQuery).arg(query.lastError().text());
    throw QtOrm::Exception(ErrorCode::Sql, errorMsg);
  }
  emit executedSql(executedQuery);
}

QList<QObject *> *Query::getList(QSqlQuery &query, const QueryModel &queryModel) {
  QList<QObject *> *objects = new QList<QObject *>();
  ClassMapBase *classBase = queryModel.getClassBase();
  QString mainTableAlias = queryModel.getMainTableModel()->getName();
  while (query.next()) {
    QObject *obj = nullptr;
    if (reestrContainsObject(*classBase, query.record(), mainTableAlias)) {
      obj = getObjectFromReestr(*classBase, query.record(), mainTableAlias);
    } else {
      obj = createNewInstance(*classBase);
      insertObjectIntoReestr(*classBase, query.record(), obj, mainTableAlias);

      fillObject(*obj, queryModel.getMainTableModel(), query.record());

      fillOneToOne(*obj, queryModel.getMainTableModel(), query.record());

      fillOneToMany(classBase->getOneToManyRelations(), classBase->getIdProperty().getName(), *obj);
    }
    objects->append(obj);
  }

  return objects;
}

void Query::fillObject(QObject &object, QueryTableModel *queryTableModel, const QSqlRecord &record) {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  auto properties = classBase->getProperties();
  for (auto prop : properties) {
    QString queryColumn = QString("%1_%2").arg(queryTableModel->getAlias()).arg(prop->getColumn());
    ;
    QVariant value = record.value(queryColumn);
    if (value.isValid() && !value.isNull())
      objectSetProperty(object, prop->getName(), value);
  }
}

void Query::fillOneToMany(const QList<OneToMany *> &relations, const QString &idProperty, QObject &object) {
  for (auto oneToMany : relations) {
    QString refClass = oneToMany->getRefClass();
    QString property = oneToMany->getProperty();
    QString refProperty = oneToMany->getRefProperty();
    QVariant value = object.property(idProperty.toStdString().data());

    QObjectList *l = getListObject(refClass, refProperty, value);

    Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(oneToMany->getRefClass());
    QVariant var = refClassBase->getVariantByObjectList(l);
    objectSetProperty(object, property, var);
  }
}

void Query::fillOneToOne(QObject &object, QueryTableModel *queryTableModel, const QSqlRecord &record) {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  for (OneToOne *oneToOne : classBase->getOneToOneRelations()) {
    QString property = oneToOne->getProperty();
    QString refClass = Mapping::ClassMapBase::getTypeNameOfProperty(object, property);

    Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(refClass);
    QObject *newObject = nullptr;

    QueryJoin *join = queryTableModel->findJoinByColumnName(oneToOne->getTableColumn());
    QString tableAlias = join->getQueryTableModel()->getAlias();
    if (reestrContainsObject(*refClassBase, record, tableAlias)) {
      newObject = getObjectFromReestr(*refClassBase, record, tableAlias);
    } else {
      newObject = createNewInstance(*refClassBase);
      insertObjectIntoReestr(*refClassBase, record, newObject, tableAlias);

      fillObject(*newObject, join->getQueryTableModel(), record);
      fillOneToOne(*newObject, join->getQueryTableModel(), record);
      fillOneToMany(refClassBase->getOneToManyRelations(), refClassBase->getIdProperty().getName(), *newObject);
    }
    QVariant var = refClassBase->getVariantByObject(newObject);
    objectSetProperty(object, property, var);
  }
}

void Query::objectSetProperty(QObject &object, const QString &propertyName, const QVariant &value) {
  if (!object.setProperty(propertyName.toStdString().data(), value)) {
    QString textError =
        QString::fromUtf8("Unable to place value in %1.%2").arg(object.metaObject()->className()).arg(propertyName);
    throw Exception(ErrorCode::UnableToSetValue, textError);
  }
}

QObject *Query::createNewInstance(ClassMapBase &classBase) {
  QObject *newObject = classBase.getMetaObject().newInstance();
  if (!newObject) {
    throw Exception(ErrorCode::InstanceNotCreated,
                    "Object instance is not created(Missing Q_INVOKABLE in constructor?)");
  }
  return newObject;
}

bool Query::reestrContainsObject(ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias) {
  QString tableName = classBase.getTable();
  QVariant idValue = getIdFromRecord(classBase, record, tableAlias);
  return reestr->contains(tableName, idValue.toString());
}

QObject *Query::getObjectFromReestr(ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias) {
  QString tableName = classBase.getTable();
  QVariant idValue = getIdFromRecord(classBase, record, tableAlias);
  return reestr->value(tableName, idValue.toString());
}

void Query::insertObjectIntoReestr(ClassMapBase &classBase, const QSqlRecord &record, QObject *object,
                                   const QString &tableAlias) {
  QString idColumn;
  if (tableAlias.isEmpty()) {
    idColumn = classBase.getIdProperty().getColumn();
  } else {
    idColumn = tableAlias + "_" + classBase.getIdProperty().getColumn();
  }

  QVariant idValue = record.value(idColumn);

  insertObjectIntoReestr(classBase, object, idValue);
}

void Query::insertObjectIntoReestr(ClassMapBase &classBase, QObject *object, QVariant idValue) {
  QString tableName = classBase.getTable();

  reestr->insert(tableName, idValue.toString(), object);
}

void Query::removeObjectFromReestr(QObject *object) {
  reestr->remove(object);
}

QVariant Query::getIdFromRecord(ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias) {
  QString idColumn = tableAlias + "_" + classBase.getIdProperty().getColumn();
  return record.value(idColumn);
}

void Query::refreshObjectData(QObject &object, QueryTableModel *queryTableModel, const QSqlRecord &record) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  fillObject(object, queryTableModel, record);

  for (OneToOne *oneToOne : classBase->getOneToOneRelations()) {
    QString oneToOnePropertyTypeName = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
    Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(oneToOnePropertyTypeName);
    QVariant propertyValue = object.property(oneToOne->getProperty().toStdString().data());
    QObject *oneToOneObj = refClassBase->getObjectByVariant(propertyValue);
    QueryJoin *join = queryTableModel->findJoinByColumnName(oneToOne->getTableColumn());
    refreshObjectData(*oneToOneObj, join->getQueryTableModel(), record);
  }

  for (OneToMany *oneToMany : classBase->getOneToManyRelations()) {
    Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(oneToMany->getRefClass());
    QVariant propertyValue = object.property(oneToMany->getProperty().toStdString().data());
    QList<QObject *> *oneToManyObjects = refClassBase->getObjectListByVariant(propertyValue);
    for (QObject *obj : *oneToManyObjects)
      refresh(*obj);
  }
}

QString Query::getSqlTextWithBindParams(QSqlQuery &query) {
  QString text;
  text = query.lastQuery() + "\n";

  auto boundValues = query.boundValues();
  for (auto it = boundValues.begin(); it != boundValues.end(); ++it)
    text += QString("%1 = %2\n").arg(it.key()).arg(it.value().toString());

  return text;
}

void Query::saveAllOneToOne(QObject &object) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  for (Mapping::OneToOne *oneToOne : classBase->getOneToOneRelations()) {
    if (oneToOne->getSaveCascade()) {
      saveOneToOne(object, oneToOne);
    }
  }
}

void Query::saveOneToOne(QObject &object, OneToOne *oneToOne) {
  QString className = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
  Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(className);

  QVariant propertyVariant = object.property(oneToOne->getProperty().toStdString().data());
  QObject *propertyObject = refClassBase->getObjectByVariant(propertyVariant);

  if (propertyObject) {
    Query subQuery(*this);
    connect(&subQuery, &Query::executedSql, this, &Query::executedSql);
    subQuery.saveObjectWoStartTransaction(*propertyObject);
  }
}

void Query::saveAllOneToMany(QObject &object) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  for (Mapping::OneToMany *oneToMany : classBase->getOneToManyRelations()) {
    if (oneToMany->getSaveCascade())
      saveOneToMany(object, oneToMany);
  }
}

void Query::saveOneToMany(QObject &object, OneToMany *oneToMany) {
  Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(oneToMany->getRefClass());
  QVariant val = object.property(oneToMany->getProperty().toStdString().data());
  QList<QObject *> *lst = refClassBase->getObjectListByVariant(val);
  for (QObject *obj : *lst) {
    Query subQuery(*this);
    connect(&subQuery, &Query::executedSql, this, &Query::executedSql);
    subQuery.saveObjectWoStartTransaction(*obj);
  }
}

Reestr *Query::getReestr() const {
  return reestr;
}

void Query::setReestr(Reestr *value) {
  reestr = value;
}

Query &Query::operator=(const Query &other) {
  reestr = other.getReestr();
  database = other.getDatabase();

  return *this;
}

void Query::saveObjectWoStartTransaction(QObject &object) {
  if (isIdObjectDefault(object))
    insertObject(object);
  else
    updateObject(object);
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

bool Query::isIdObjectDefault(QObject &object) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  QVariant idVal = object.property(classBase->getIdProperty().getName().toStdString().data());
  return idVal.value<ulong>() == 0;
}

bool Query::isIdOneToOneDefault(QObject &object, OneToOne *oneToOne) {
  QString className = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
  Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(className);

  QVariant propertyVariant = object.property(oneToOne->getProperty().toStdString().data());
  QObject *propertyObject = refClassBase->getObjectByVariant(propertyVariant);

  return isIdObjectDefault(*propertyObject);
}
}
}
