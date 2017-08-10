#include "Query.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <QSharedPointer>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QTime>

#include "ConfigurationMap.h"
#include "SimpleSqlBuilder.h"

#include "Conditions/Condition.h"

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

  if (registry->contains(classBase->getTable(), id.toString()))
    return registry->value(classBase->getTable(), id.toString());

  auto list = getListObject(className, classBase->getIdPropertyName(), id);

  if (!list.size()) {
    return QSharedPointer<QObject>();
  }

  if (list.size() > 1)
    throw FindMoreThatOneRecordException(
        QString::fromUtf8("Found %1 records has id equal %2").arg(list.size()).arg(id.toString()));

  return list.takeFirst();
}

QList<QSharedPointer<QObject>> Query::getListObject(const QString &className, const QString &property,
                                                    const QVariant &value, const QList<OrderColumn> &orderBy) {
  GroupConditions group;
  if (!property.isEmpty()) {
    QSharedPointer<Condition> condition =
        QSharedPointer<Condition>::create(property, Operation::Equal, QVariantList{value});
    group.addCondition(condition);
  }

  return getListObject(className, group, orderBy);
}

QList<QSharedPointer<QObject>> Query::getListObject(const QString &className, const GroupConditions &conditions,
                                                    const QList<OrderColumn> &orderBy) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(className);
  QVariant discriminatorValue;
  if(SubClassMap::isClassTableInheritance(classBase)){
    discriminatorValue = classBase->getDiscriminatorValue();
    classBase = classBase->toSubclass()->getBaseClass();
  } else {
    discriminatorValue = classBase->getDiscriminatorValue();
  }

  QList<OrderColumn> replacedOrderBy = replacePropertyToColumn(classBase, orderBy);

  GroupConditions resultWhere;
  if (classBase->isSubclass()) {
    resultWhere.addEqual(classBase->getDiscriminatorColumn(), discriminatorValue);
  }

  GroupConditions replacedConditions = replacePropertyToColumn(classBase, conditions);
  if (!replacedConditions.isEmpty()) {
    resultWhere.addGroup(replacedConditions);
  }

  SimpleSqlBuilder sqlBuilder = createSimpleSqlBuilder(classBase, resultWhere, replacedOrderBy);
  QSqlQuery query = sqlBuilder.selectQuery();
  executeQuery(query);

  return getList(query, sqlBuilder.getQueryModel());
}

void Query::saveObject(QSharedPointer<QObject> &object) {
  if (isIdObjectDefault(object)) {
    insertObject(object);
  } else {
    updateObject(object);
  }
}

void Query::insertObject(QSharedPointer<QObject> &object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  if(SubClassMap::isClassTableInheritance(classBase)){
    insertObjectCti(object, classBase);
  } else {
    insertObjectMain(object, classBase);
  }
}

QSqlQuery Query::insertBase(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase>& classBase) {
  saveAllOneToOne(object);

  SimpleSqlBuilder sqlBuilder = createSimpleSqlBuilder(classBase, object);
  QSqlQuery query = sqlBuilder.insertQuery();
  executeQuery(query);

  return query;
}

void Query::insertObjectMain(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase>& classBase) {
  QSqlQuery query = insertBase(object, classBase);

  QVariant newId;
  if (database.driver()->hasFeature(QSqlDriver::LastInsertId) && database.driverName() != "QPSQL") {
    newId = query.lastInsertId();
  } else {
    query.first();
    newId = query.record().value(0);
  }
  setObjectProperty(object, classBase->getIdPropertyName(), newId);
  insertObjectIntoRegistry(classBase, object, newId);

  saveAllOneToMany(object);

  connectToAllProperties(object);
}

void Query::insertObjectCti(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase> &classBase) {
  auto superClass = classBase->toSubclass()->getSuperClass();
  if(superClass->isSubclass()){
    insertObjectCti(object, superClass);
  } else {
    insertObjectMain(object, superClass);
  }

  insertBase(object, classBase);
  saveAllOneToMany(object);
}

void Query::updateObject(QSharedPointer<QObject> &object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  if(SubClassMap::isClassTableInheritance(classBase)){
    updateObjectCti(object, classBase);
  } else {
    updateObjectMain(object, classBase);
  }
}

void Query::updateObjectMain(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase>&classBase) {
  saveAllOneToOne(object);

  SimpleSqlBuilder sqlBuilder = createSimpleSqlBuilder(classBase, object);
  QSqlQuery query = sqlBuilder.updateQuery();
  executeQuery(query);

  saveAllOneToMany(object);
}

void Query::updateObjectCti(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase>&classBase) {
  auto superClass = classBase->toSubclass()->getSuperClass();
  if(superClass->isSubclass()){
    updateObjectCti(object, superClass);
  } else {
    updateObjectMain(object, superClass);
  }

  updateObjectMain(object, classBase);
}

void Query::deleteObjectBase(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase> &classBase) {
  deleteAllOneToMany(object);

  SimpleSqlBuilder sqlBuilder = createSimpleSqlBuilder(classBase, object);
  QSqlQuery query = sqlBuilder.deleteQuery();
  executeQuery(query);
}

void Query::deleteObject(QSharedPointer<QObject> &object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  if(SubClassMap::isClassTableInheritance(classBase)){
    deleteObjectCti(object, classBase);
  } else {
    deleteObjectMain(object, classBase);
  }
}

void Query::deleteObjectMain(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase> &classBase) {
  deleteObjectBase(object, classBase);
  removeObjectFromRegistry(object);
}

void Query::deleteObjectCti(QSharedPointer<QObject>&object, QSharedPointer<ClassMapBase> &classBase) {
  deleteObjectBase(object, classBase);

  auto superClass = classBase->toSubclass()->getSuperClass();
  if(superClass->isSubclass()){
    deleteObjectCti(object, superClass);
  } else {
    deleteObjectMain(object, superClass);
  }
}

void Query::refresh(QSharedPointer<QObject> &object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  if(SubClassMap::isClassTableInheritance(classBase)){
    classBase = classBase->toSubclass()->getBaseClass();
  }

  QSharedPointer<PropertyMap> property = classBase->getIdProperty();
  QString idPropertyName = property->getName();
  QVariant idPropertyValue = object->property(idPropertyName.toStdString().data());
  GroupConditions group;
  group.addEqual(idPropertyName, idPropertyValue);

  SimpleSqlBuilder sqlBuilder = createSimpleSqlBuilder(classBase, group);
  QSqlQuery query = sqlBuilder.selectQuery();
  executeQuery(query);

  query.next();
  QSqlRecord record = query.record();
  QString idColumn = getQueryColumn(sqlBuilder.getQueryModel()->getMainTableModel(), property);
  if (!record.value(idColumn).isNull()) {
    refreshObjectData(object, classBase, sqlBuilder.getQueryModel()->getMainTableModel(), record);
  }
}

void Query::saveOneField(QSharedPointer<QObject> &object, const QString &propertyName) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  if(SubClassMap::isClassTableInheritance(classBase)){
    QString propertyOwnerClassName = classBase->toSubclass()->getClassNameByProperty(propertyName);
    classBase = ConfigurationMap::getMappedClass(propertyOwnerClassName);
  }
  QSharedPointer<OneToMany> oneToMany = classBase->findOneToManyByPropertyName(propertyName);
  if (oneToMany) {
    saveOneToMany(object, oneToMany);
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    if (oneToOne && isIdOneToOneDefault(object, oneToOne)) {
      saveOneToOne(object, oneToOne);
    }

    SimpleSqlBuilder sqlBuilder = createSimpleSqlBuilder(classBase, object);
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
  if (tryReopenDatabaseConnectionIfNeed()) {
    throw DatabaseConnectionClosedException("Database connection closed");
  }

  QString executedQuery = getSqlTextWithBindParams(query);
  QTime time;
  QTime startTime = QTime::currentTime();
  time.start();
  if (!query.exec()) {
    emit executedSql(executedQuery);
    QString errorMsg = QString("Query: %1 \nError: %2").arg(executedQuery).arg(query.lastError().text());
    throw SqlException(errorMsg);
  }
  int timeElapsed = time.elapsed();
  QString fullExecutedMessage = QString("[%1](%2ms) %3").arg(startTime.toString("hh:mm:ss:zzzz")).arg(timeElapsed).arg(executedQuery);
  emit executedSql(fullExecutedMessage);
}

QList<QSharedPointer<QObject>> Query::getList(QSqlQuery &query, const QSharedPointer<QueryModel> &queryModel) {
  QList<QSharedPointer<QObject>> objects;
  QSharedPointer<ClassMapBase> classBase = queryModel->getClassBase();
  QString mainTableAlias = queryModel->getMainTableModel()->getAlias();
  while (query.next()) {
    QSharedPointer<QObject> newObject =
        getObject(query.record(), classBase, mainTableAlias, queryModel->getMainTableModel());
    objects.append(newObject);
  }

  return objects;
}

void Query::fillObject(QSharedPointer<QObject> &object, const QSharedPointer<ClassMapBase>& baseMap, QSharedPointer<QueryTableModel>& tableModel, const QSqlRecord &record) {
  fillBaseFields(object, baseMap, tableModel, record);
  fillOneToOne(object, tableModel, record, baseMap->getOneToOneRelations());
  fillOneToMany(object, baseMap->getOneToManyRelations(), baseMap->getIdPropertyName());
}

void Query::fillBaseFields(QSharedPointer<QObject> &object, const QSharedPointer<ClassMapBase>& classBase, QSharedPointer<QueryTableModel> &queryTableModel,
                       const QSqlRecord &record) {
  QMap<QString, QSharedPointer<PropertyMap>> properties = classBase->getProperties();
  for (auto prop : properties) {
    QString queryColumn = getQueryColumn(queryTableModel, prop);
    QVariant value = record.value(queryColumn);
    QVariant nullValue = prop->getNull();
    if(value.isNull() && nullValue.isValid()) {
      value = nullValue;
    }
    if (value.isValid()) {
      setObjectProperty(object, prop->getName(), value);
    }
  }
}

void Query::fillCtiPart(QSharedPointer<QObject> &object, const QSharedPointer<ClassMapBase>& baseMap, const QSharedPointer<ClassMapBase>& subMap) {
  QVariant idValue = object->property(baseMap->getIdPropertyName().toStdString().c_str());
  auto currentMap = subMap;
  while (currentMap->isSubclass()) {
    GroupConditions where;
    where.addEqual(currentMap->getIdColumn(), idValue);
    SimpleSqlBuilder sqlBuilder = createSimpleSqlBuilder(currentMap, where);
    QSqlQuery query = sqlBuilder.selectQuery();
    executeQuery(query);
    query.next();
    auto subRecord = query.record();
    auto subQueryModel = sqlBuilder.getQueryModel();
    auto subMainTableModel = subQueryModel->getMainTableModel();

    fillObject(object, currentMap, subMainTableModel, subRecord);

    currentMap = currentMap->toSubclass()->getSuperClass();
  }
}

void Query::fillOneToMany(QSharedPointer<QObject> object, const QList<QSharedPointer<OneToMany>> &relations,
                          const QString &idProperty) {
  for (QSharedPointer<OneToMany> oneToMany : relations) {
    QString property = oneToMany->getProperty();
    QString refClass = Mapping::ClassMapBase::getTypeNameOfProperty(object, property);
    QString column = oneToMany->getColumn();
    QVariant value = object->property(idProperty.toStdString().data());

    GroupConditions where;
    where.addCondition(column, Operation::Equal, QVariantList{value});
    if (!oneToMany->getCondition().isEmpty())
      where.addGroup(oneToMany->getCondition());

    QList<QSharedPointer<QObject>> qobjectList = getListObject(refClass, where, oneToMany->getOrderBy());

    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(refClass);
    auto kind = Mapping::ClassMapBase::getTypeKindOfProperty(object, property);
    QVariant var = refClassBase->castToList(kind, qobjectList);
    setObjectProperty(object, property, var);
  }
}

void Query::fillOneToOne(QSharedPointer<QObject> object, QSharedPointer<QueryTableModel> queryTableModel,
                         const QSqlRecord &record, QList<QSharedPointer<OneToOne>> relations) {
  for (QSharedPointer<OneToOne> oneToOne : relations) {
    QString property = oneToOne->getProperty();
    QString refClass = Mapping::ClassMapBase::getTypeNameOfProperty(object, property);

    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(refClass);

    QSharedPointer<QueryJoin> join = queryTableModel->findJoinByColumnName(oneToOne->getTableColumn());
    QString tableAlias = join->getQueryTableModel()->getAlias();
    QString idColumn = getQueryColumn(join->getQueryTableModel(), refClassBase->getIdProperty());
    QVariant idValue = record.value(idColumn);
    if (!idValue.isNull()) {
      bool isBase = refClassBase->isBaseClass();
      QSharedPointer<QObject> newObject;
      if (isBase) {
        newObject = getById(refClass, idValue);
      } else {
        newObject = getObject(record, refClassBase, tableAlias, join->getQueryTableModel());
      }

      auto kind = Mapping::ClassMapBase::getTypeKindOfProperty(object, property);
      QVariant var = refClassBase->castTo(kind, newObject);
      setObjectProperty(object, property, var);
    }
  }
}

QSharedPointer<QObject> Query::getObject(const QSqlRecord &record, const QSharedPointer<ClassMapBase> &classBase,
                                         const QString &tableAlias, QSharedPointer<QueryTableModel> queryTableModel) {
  QSharedPointer<QObject> object;

  if (registryContainsObject(classBase, record, tableAlias)) {
    object = getObjectFromRegistry(classBase, record, tableAlias);
  } else {
    object = createNewInstance(classBase, record, queryTableModel);
    insertObjectIntoRegistry(classBase, record, object, tableAlias);

    fillObject(object, classBase, queryTableModel, record);

    auto currentClassBase = ConfigurationMap::getMappedClass(object->metaObject()->className());
    if(SubClassMap::isClassTableInheritance(currentClassBase)) {
      fillCtiPart(object, classBase, currentClassBase);
    }

    connectToAllProperties(object);
  }

  return object;
}

void Query::setObjectProperty(QSharedPointer<QObject> object, const QString &propertyName, QVariant &value) {
  const QMetaObject *metaObject = object->metaObject();
  int propertyIndex = metaObject->indexOfProperty(propertyName.toStdString().data());
  if (metaObject->property(propertyIndex).isEnumType()) {
    value = value.value<int>();
  }
  if (!object->setProperty(propertyName.toStdString().data(), value)) {
    QString textError = QString::fromUtf8("Unable to place value %3 in %1.%2")
                            .arg(object->metaObject()->className())
                            .arg(propertyName)
                            .arg(value.toString());
    qDebug() << textError;
    //    throw UnableToSetValueException(textError);
  }
}

QSharedPointer<QObject> Query::createNewInstance(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                                 const QSharedPointer<QueryTableModel> &model) {
  QSharedPointer<QObject> newObject;
  if (!classBase->isBaseClass()) {
    newObject = QSharedPointer<QObject>(classBase->getMetaObject().newInstance());
  } else {
    QString discrimanatorColumn = getQueryColumn(model, classBase->getDiscriminatorProperty());
    QVariant recordValue = record.value(discrimanatorColumn);
    for (auto dc : classBase->getDerrivedClasses()) {
      QVariant discrimanatorValue = dc->getDiscriminatorValue();
      if (recordValue == discrimanatorValue) {
        newObject = QSharedPointer<QObject>(dc->getMetaObject().newInstance());
        break;
      }
    }
  }

  if (!newObject) {
    QString message = QString("Object(%1) instance did not created(Missed Q_INVOKABLE in constructor?)")
                          .arg(classBase->getClassName());
    throw InstanceNotCreatedException(message);
  }

  return newObject;
}

bool Query::registryContainsObject(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                   const QString &tableAlias) {
  QString tableName = classBase->getTable();
  QVariant idValue = getIdFromRecord(classBase, record, tableAlias);
  return registry->contains(tableName, idValue.toString());
}

QSharedPointer<QObject> Query::getObjectFromRegistry(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                                     const QString &tableAlias) {
  QString tableName = classBase->getTable();
  QVariant idValue = getIdFromRecord(classBase, record, tableAlias);
  return registry->value(tableName, idValue.toString());
}

void Query::insertObjectIntoRegistry(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                     QSharedPointer<QObject> object, const QString &tableAlias) {
  QVariant idValue = getIdFromRecord(classBase, record, tableAlias);
  insertObjectIntoRegistry(classBase, object, idValue);
}

void Query::insertObjectIntoRegistry(QSharedPointer<ClassMapBase> classBase, QSharedPointer<QObject> object,
                                     QVariant idValue) {
  QString tableName = classBase->getTable();
  registry->insert(tableName, idValue.toString(), object);
}

void Query::removeObjectFromRegistry(QSharedPointer<QObject> object) {
  registry->remove(object);
}

QVariant Query::getIdFromRecord(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                const QString &tableAlias) {
  QString idColumn = classBase->getIdColumn();
  if (!tableAlias.isEmpty())
    idColumn = tableAlias + "_" + idColumn;

  return record.value(idColumn);
}

void Query::refreshObjectData(QSharedPointer<QObject> object, QSharedPointer<ClassMapBase>& classBase, QSharedPointer<QueryTableModel> queryTableModel,
                              const QSqlRecord &record) {
  fillBaseFields(object, classBase, queryTableModel, record);

  refreshedObject.append(object);

  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()) {
    QVariant propertyValue = object->property(oneToOne->getProperty().toStdString().data());
    QObject* qobjectPropertyValue = propertyValue.value<QObject*>();
    if(qobjectPropertyValue) {
      QSharedPointer<QObject> oneToOneObj = registry->value(qobjectPropertyValue);
      if (!refreshedObject.contains(oneToOneObj)) {
        QSharedPointer<ClassMapBase> refClassMap = ConfigurationMap::getMappedClass(oneToOneObj);
        if(SubClassMap::isClassTableInheritance(refClassMap)){
          refClassMap = refClassMap->toSubclass()->getBaseClass();
        }
        QSharedPointer<QueryJoin> join = queryTableModel->findJoinByColumnName(oneToOne->getTableColumn());
        refreshObjectData(oneToOneObj, refClassMap, join->getQueryTableModel(), record);
      }
    }
  }

  auto currentClassBase = ConfigurationMap::getMappedClass(object->metaObject()->className());
  if(SubClassMap::isClassTableInheritance(currentClassBase)) {
    fillCtiPart(object, classBase, currentClassBase);
  }

  QVariant idValue = object->property(classBase->getIdProperty()->getName().toStdString().data());

  for (QSharedPointer<OneToMany> oneToMany : classBase->getOneToManyRelations()) {
    QString oneToManyPropertyTypeName = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToMany->getProperty());
    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(oneToManyPropertyTypeName);
    QString refColumn = oneToMany->getColumn();
    QList<QSharedPointer<QObject>> newChildren = getListObject(refClassBase->getClassName(), refColumn, idValue);

    auto kind = Mapping::ClassMapBase::getTypeKindOfProperty(object, oneToMany->getProperty());
    QVariant newChildrenVariant = refClassBase->castToList(kind, newChildren);

    QVariant propertyValue = object->property(oneToMany->getProperty().toStdString().data());
    QVariantList currentChildren = propertyValue.value<QVariantList>();

    for (QVariant& child : newChildrenVariant.value<QVariantList>()) {
      auto childShared = registry->value(child.value<QObject*>());
      if (currentChildren.contains(child) && !refreshedObject.contains(childShared)) {
        refresh(childShared);
      }
    }

    QVariant newChildrenValue = refClassBase->castToList(kind, newChildren);
    object->setProperty(oneToMany->getProperty().toStdString().data(), newChildrenValue);
  }
}

QString Query::getSqlTextWithBindParams(QSqlQuery &query) {
  QString text;
  text = query.lastQuery() + "\n";

  auto boundValues = query.boundValues();
  for (auto it = boundValues.begin(); it != boundValues.end(); ++it) {
    QString value;
    if (!it.value().isNull() || it.value().isValid()) {
      value = it.value().toString();
    } else {
      value = "null";
    }
    text += QString("%1 = %2\n").arg(it.key()).arg(value);
  }

  return text;
}

void Query::saveAllOneToOne(QSharedPointer<QObject> object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()) {
    if (oneToOne->getSaveCascade()) {
      saveOneToOne(object, oneToOne);
    }
  }
}

void Query::saveOneToOne(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne) {
  QVariant propertyVariant = object->property(oneToOne->getProperty().toStdString().data());
  QObject* qobjectPropertyValue = propertyVariant.value<QObject*>();
  QSharedPointer<QObject> propertyObject = registry->value(qobjectPropertyValue);

  if (propertyObject) {
    Query subQuery(*this);
    connect(&subQuery, &Query::executedSql, this, &Query::executedSql);
    subQuery.saveObject(propertyObject);
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
  QString property = oneToMany->getProperty();
  QVariant val = object->property(property.toStdString().data());
  TypeKind type = ClassMapBase::getTypeKindOfProperty(object, property);
  QString propertyTypeName = ClassMapBase::getTypeNameOfProperty(object, property);
  QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(propertyTypeName);
  QList<QSharedPointer<QObject>> list = refClassBase->castFromList(type, val);

  for (auto item : list) {
    Query subQuery(*this);
    connect(&subQuery, &Query::executedSql, this, &Query::executedSql);
    subQuery.saveObject(item);
  }
}

void Query::deleteAllOneToMany(QSharedPointer<QObject> object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  for (auto oneToMany : classBase->getOneToManyRelations()) {
    if (oneToMany->getDeleteCascade()) {
      deleteOneToMany(object, oneToMany);
    }
  }
}

void Query::deleteOneToMany(QSharedPointer<QObject> object, const QSharedPointer<OneToMany> &oneToMany) {
  QVariant val = object->property(oneToMany->getProperty().toStdString().data());

  QSequentialIterable iterable = val.value<QSequentialIterable>();
  for (const QVariant &variantItem : iterable) {
    Query subQuery(*this);
    connect(&subQuery, &Query::executedSql, this, &Query::executedSql);
    auto qobject = variantItem.value<QObject*>();
    auto sharedObject = registry->value(qobject);
    subQuery.deleteObject(sharedObject);
  }
}

QSharedPointer<Registry> Query::getRegistry() const {
  return registry;
}

void Query::setRegistry(QSharedPointer<Registry> value) {
  registry = value;
}

Query &Query::operator=(const Query &other) {
  updater = other.getUpdater();
  registry = other.getRegistry();
  database = other.getDatabase();

  return *this;
}

bool Query::isIdObjectDefault(QSharedPointer<QObject> object) {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass(object);
  QVariant idVal = object->property(classBase->getIdPropertyName().toStdString().data());
  return idVal == classBase->getIdProperty()->getNull();
}

bool Query::isIdOneToOneDefault(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne) {
  QVariant propertyVariant = object->property(oneToOne->getProperty().toStdString().data());
  QObject* objectPropertyValue = propertyVariant.value<QObject*>();
  QSharedPointer<QObject> propertyObject = registry->value(objectPropertyValue);

  return isIdObjectDefault(propertyObject);
}

QString Query::getQueryColumn(QSharedPointer<QueryTableModel> queryTableModel, QSharedPointer<PropertyMap> property) {
  return QString("%1_%2").arg(queryTableModel->getAlias()).arg(property->getColumn());
}

bool Query::tryReopenDatabaseConnectionIfNeed() {
  return !database.isOpen() && !database.open();
}

SimpleSqlBuilder Query::createSimpleSqlBuilder(QSharedPointer<ClassMapBase>&classBase, GroupConditions& conditions, QList<OrderColumn>&orderBy) {
  auto sqlBuilder = createSimpleSqlBuilder(classBase);
  sqlBuilder.setConditions(conditions);
  sqlBuilder.setOrderBy(orderBy);
  return sqlBuilder;
}

SimpleSqlBuilder Query::createSimpleSqlBuilder(QSharedPointer<ClassMapBase>&classBase, GroupConditions& conditions) {
  auto sqlBuilder = createSimpleSqlBuilder(classBase);
  sqlBuilder.setConditions(conditions);
  return sqlBuilder;
}

SimpleSqlBuilder Query::createSimpleSqlBuilder(QSharedPointer<ClassMapBase> &classBase, QSharedPointer<QObject>&object) {
  auto sqlBuilder = createSimpleSqlBuilder(classBase);
  sqlBuilder.setObject(object);
  return sqlBuilder;
}

SimpleSqlBuilder Query::createSimpleSqlBuilder(QSharedPointer<ClassMapBase> &classBase) {
  SimpleSqlBuilder sqlBuilder;
  sqlBuilder.setDatabase(database);
  sqlBuilder.setClassBase(classBase);

  return sqlBuilder;
}

GroupConditions Query::replacePropertyToColumn(QSharedPointer<ClassMapBase> &classBase,
                                               const GroupConditions &conditions) {
  GroupConditions result;

  if (conditions.isEmpty())
    return result;

  result.setOperation(conditions.getOperation());

  for (QSharedPointer<Condition> &condition : conditions.getConditions()) {
    QSharedPointer<Condition> newCondition = QSharedPointer<Condition>::create(*condition.data());
    newCondition->setProperty(getColumn(classBase, newCondition->getProperty()));
    result.addCondition(newCondition);
  }

  for (QSharedPointer<GroupConditions> &group : conditions.getGroups()) {
    result.addGroup(replacePropertyToColumn(classBase, *group.data()));
  }

  return result;
}

QList<OrderColumn> Query::replacePropertyToColumn(QSharedPointer<ClassMapBase> &classBase,
                                                  const QList<OrderColumn> &orderColumns) {
  if (orderColumns.isEmpty())
    return orderColumns;

  QList<OrderColumn> result = orderColumns;
  for (auto orderColumn : result) {
    orderColumn.orderProperty = getColumn(classBase, orderColumn.orderProperty);
  }

  return result;
}

QString Query::getColumn(QSharedPointer<ClassMapBase> &classBase, const QString &property) {
  if (classBase->containsProperty(property)) {
    return classBase->getPropertyColumn(property);
  }

  return property;
}

void Query::connectToAllProperties(QSharedPointer<QObject> &object) {
  if (updater) {
    updater->connectToAllProperties(object);
  }
}

QSharedPointer<AutoUpdater> Query::getUpdater() const {
  return updater;
}

void Query::setUpdater(const QSharedPointer<AutoUpdater> &value) {
  updater = value;
}

void Query::clearRefreshedObject() {
  refreshedObject.clear();
}
}
}
