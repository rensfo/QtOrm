#include "Query.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>

#include "QDebug"

#include "ConfigurationMap.h"

namespace QtOrm {
namespace Sql {

using namespace Config;
Query::Query(QObject *parent) : QObject(parent) {
  onObjectPropertyChangedMethod = findOnObjectPropertyChangedMethod();
}

QObject *Query::getById(const QString &className, const QVariant &id) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(className);

  auto list = getListObject(className, classBase->getIdProperty().getName(), id);

  if (list->size() == 0) {
    return nullptr;
    //    throw Exception(ErrorGroup::Sql, QString::fromUtf8("Data not found"));
  }

  if (list->size() > 1)
    throw Exception(ErrorGroup::Sql,
                    QString::fromUtf8("Found %1 records has id equal %2").arg(list->size()).arg(id.toString()));

  return list->takeFirst();
}

QList<QObject *> *Query::getListObject(const QString &className, const QString property, const QVariant value) {
  GroupConditions group;
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(className);
  if (!property.isEmpty()) {
    QString column = classBase->getProperty(property).getColumn();
    Condition filter(this);
    filter.setFieldName(column);
    filter.setOperation(Operation::Equal);
    filter.setValue(value);
    group.addFilter(filter);
  }

  return getListObject(className, group);
}

QList<QObject *> *Query::getListObject(const QString &className, const GroupConditions &conditions) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(className);
  QString mainTableAlias = sqlBuilder->generateTableAlias();
  sqlBuilder->setClassBase(classBase);
  sqlBuilder->setTableAlias(mainTableAlias);
  sqlBuilder->setConditions(conditions);
  QSqlQuery query = sqlBuilder->selectQuery();
  oneToOneAlias = sqlBuilder->getOneToOneAlias();

  executeQuery(query);

  return getList(*classBase, query, mainTableAlias);
}

void Query::saveObject(QObject &object) {
  startTransaction();
  saveObjectWoStartTransaction(object);
  commit();
}

void Query::insertObject(QObject &object) {
  saveAllOneToOne(object);

  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  QString mainTableAlias = sqlBuilder->generateTableAlias();
  sqlBuilder->setClassBase(classBase);
  sqlBuilder->setTableAlias(mainTableAlias);
  sqlBuilder->setObject(&object);
  QSqlQuery query = sqlBuilder->insertQuery();

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
  if (autoUpdate) {
    connectToProperties(object, *classBase);
  }

  saveAllOneToMany(object);
}

void Query::updateObject(QObject &object) {
  saveAllOneToOne(object);

  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  QString mainTableAlias = sqlBuilder->generateTableAlias();
  sqlBuilder->setClassBase(classBase);
  sqlBuilder->setTableAlias(mainTableAlias);
  sqlBuilder->setObject(&object);
  QSqlQuery query = sqlBuilder->updateQuery();

  executeQuery(query);

  saveAllOneToMany(object);
}

void Query::deleteObject(QObject &object) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());

  QString mainTableAlias = sqlBuilder->generateTableAlias();
  sqlBuilder->setClassBase(classBase);
  sqlBuilder->setTableAlias(mainTableAlias);
  sqlBuilder->setObject(&object);
  QSqlQuery query = sqlBuilder->deleteQuery();

  executeQuery(query);

  removeObjectFromReestr(&object);
}

void Query::refresh(QObject &object) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  QString mainTableAlias = sqlBuilder->generateTableAlias();
  GroupConditions group;
  group.addFilterEqual(classBase->getIdProperty().getColumn(),
                       object.property(classBase->getIdProperty().getName().toStdString().data()));
  sqlBuilder->setClassBase(classBase);
  sqlBuilder->setTableAlias(mainTableAlias);
  sqlBuilder->setConditions(group);
  QSqlQuery query = sqlBuilder->selectQuery();
  query.next();
  QSqlRecord record = query.record();
  refreshObjectData(object, record, mainTableAlias);
}

QSqlDatabase Query::getDatabase() const {
  return database;
}

void Query::setDatabase(const QSqlDatabase &value) {
  database = value;
  if (sqlBuilder)
    sqlBuilder->setDatabase(database);
}

void Query::executeQuery(QSqlQuery &query) {
  QString executedQuery = getSqlTextWithBindParams(query);
  if (!query.exec()) {
    emit executeSql(executedQuery);
    QString errorMsg = QString("Query: %1 \nError: %2").arg(executedQuery).arg(query.lastError().text());
    qDebug() << errorMsg;
    throw QtOrm::Exception(ErrorGroup::Sql, errorMsg);
  }
  emit executeSql(executedQuery);
}

QList<QObject *> *Query::getList(ClassMapBase &classBase, QSqlQuery &query, const QString &mainTableAlias) {
  QList<QObject *> *objects = new QList<QObject *>();
  while (query.next()) {
    QObject *obj = nullptr;
    if (reestrContainsObject(classBase, query.record(), mainTableAlias)) {
      obj = getObjectFromReestr(classBase, query.record(), mainTableAlias);
    } else {
      obj = createNewInstance(classBase);
      insertObjectIntoReestr(classBase, query.record(), obj, mainTableAlias);

      fillObject(classBase, query.record(), mainTableAlias, *obj);

      auto oneToOneRelations = classBase.getOneToOneRelations();
      fillOneToOne(oneToOneRelations, *obj, query.record());

      fillOneToMany(classBase.getOneToManyRelations(), classBase.getIdProperty().getName(), *obj);

      if (autoUpdate) {
        connectToProperties(*obj, classBase);
      }
    }
    objects->append(obj);
  }

  return objects;
}

void Query::fillObject(const ClassMapBase &classBase, const QSqlRecord &record, const QString tableAlias,
                       QObject &object) {
  auto properties = classBase.getProperties();
  for (auto prop : properties) {
    QString queryColumn;
    if (tableAlias.isEmpty())
      queryColumn = prop->getColumn();
    else
      queryColumn = QString("%1_%2").arg(tableAlias).arg(prop->getColumn());
    QVariant value = record.value(queryColumn);
    if (value.isValid() && !value.isNull())
      objectSetProperty(object, prop->getName(), value);
  }
}

void Query::fillOneToMany(const QList<OneToMany *> &relations, const QString &idProperty, QObject &object) {
  foreach (auto oneToMany, relations) {
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

void Query::fillOneToOne(QList<OneToOne *> &relations, QObject &object, const QSqlRecord &record) {
  for (OneToOne *oneToOne : relations) {
    QString property = oneToOne->getProperty();
    QString refClass = SqlBuilderBase::getTypeNameOfProperty(object, property);

    Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(refClass);
    QString queryColumn;
    if (oneToOneAlias.contains(oneToOne)) {
      queryColumn = QString("%1_%2").arg(oneToOneAlias.value(oneToOne)).arg(refClassBase->getIdProperty().getColumn());
    } else {
      queryColumn = refClassBase->getIdProperty().getColumn();
    }

    if (!record.field(queryColumn).isNull()) {
      QObject *newObject = nullptr;
      QString tableAlias = oneToOneAlias.value(oneToOne);

      if (reestrContainsObject(*refClassBase, record, tableAlias)) {
        newObject = getObjectFromReestr(*refClassBase, record, oneToOneAlias.value(oneToOne));
      } else {
        newObject = createNewInstance(*refClassBase);
        insertObjectIntoReestr(*refClassBase, record, newObject, tableAlias);

        fillObject(*refClassBase, record, tableAlias, *newObject);

        auto oneToOneRelations = refClassBase->getOneToOneRelations();
        fillOneToOne(oneToOneRelations, *newObject, record);
        fillOneToMany(refClassBase->getOneToManyRelations(), refClassBase->getIdProperty().getName(), *newObject);
        if (autoUpdate) {
          connectToProperties(*newObject, *refClassBase);
        }
      }
      QVariant var = refClassBase->getVariantByObject(newObject);
      objectSetProperty(object, property, var);
    }
  }
}

void Query::objectSetProperty(QObject &object, const QString &propertyName, const QVariant &value) {
  if (!object.setProperty(propertyName.toStdString().data(), value)) {
    QString textError =
        QString::fromUtf8("Unable to place value in %1.%2").arg(object.metaObject()->className()).arg(propertyName);
    throw Exception(ErrorGroup::MetaData, textError);
  }
}

QObject *Query::createNewInstance(ClassMapBase &classBase) {
  QObject *newObject = classBase.getMetaObject().newInstance();
  if (!newObject) {
    throw Exception(ErrorGroup::MetaData, "Object instance do not created(Missing Q_INVOKABLE in constructor?)");
  }
  return newObject;
}

bool Query::reestrContainsObject(ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias) {
  auto reestrKey = getReestrKey(classBase, record, tableAlias);
  return reestr.contains(reestrKey);
}

QObject *Query::getObjectFromReestr(ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias) {
  auto reestrKey = getReestrKey(classBase, record, tableAlias);
  return reestr[reestrKey];
}

void Query::insertObjectIntoReestr(ClassMapBase &classBase, const QSqlRecord &record, QObject *object,
                                   const QString &tableAlias) {
  QString idColumn;
  if (tableAlias.isEmpty())
    idColumn = classBase.getIdProperty().getColumn();
  else
    idColumn = tableAlias + "_" + classBase.getIdProperty().getColumn();

  QVariant idValue = record.value(idColumn);

  insertObjectIntoReestr(classBase, object, idValue);
}

void Query::insertObjectIntoReestr(ClassMapBase &classBase, QObject *object, QVariant idValue) {
  QString tableName = classBase.getTable();
  QPair<QString, QString> reestrKey(tableName, idValue.toString());

  reestr.insert(reestrKey, object);
}

void Query::removeObjectFromReestr(QObject *object) {
  reestr.remove(reestr.key(object));
}

QPair<QString, QString> Query::getReestrKey(ClassMapBase &classBase, const QSqlRecord &record,
                                            const QString &tableAlias) {
  QString tableName = classBase.getTable();
  QString idColumn = tableAlias + "_" + classBase.getIdProperty().getColumn();
  QString idValue = record.value(idColumn).toString();

  return QPair<QString, QString>(tableName, idValue);
}

void Query::refreshObjectData(QObject &object, const QSqlRecord &record, const QString tableAlias) {
  Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(object.metaObject()->className());
  fillObject(*classBase, record, tableAlias, object);

  for (OneToOne *oneToOne : classBase->getOneToOneRelations()) {
    Mapping::ClassMapBase *refClassBase =
        ConfigurationMap::getMappedClass(SqlBuilderBase::getTypeNameOfProperty(object, oneToOne->getProperty()));
    QVariant propertyValue = object.property(oneToOne->getProperty().toStdString().data());
    QObject *oneToOneObj = refClassBase->getObjectByVariant(propertyValue);
    refreshObjectData(*oneToOneObj, record, oneToOneAlias[oneToOne]);
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
  QString className = sqlBuilder->getTypeNameOfProperty(object, oneToOne->getProperty());
  Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(className);

  QVariant propertyVariant = object.property(oneToOne->getProperty().toStdString().data());
  QObject *propertyObject = refClassBase->getObjectByVariant(propertyVariant);
  if (propertyObject)
    saveObjectWoStartTransaction(*propertyObject);
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
    saveObjectWoStartTransaction(*obj);
  }
}

void Query::connectToProperties(QObject &object, const ClassMapBase &classBase) {
  QStringList properties;
  for (OneToOne *oneToOne : classBase.getOneToOneRelations())
    properties.append(oneToOne->getProperty());

  properties.append(classBase.getProperties().keys());

  for (QString propertyName : properties) {
    int propertyIndex = object.metaObject()->indexOfProperty(propertyName.toStdString().data());
    QMetaProperty property = object.metaObject()->property(propertyIndex);
    if (property.hasNotifySignal()) {
      connect(&object, property.notifySignal(), this, onObjectPropertyChangedMethod);
    } else {
      if (classBase.getIdProperty().getName() != propertyName)
        qDebug() << QString("Property %1 from class %2 has not Notify signal!")
                        .arg(propertyName)
                        .arg(object.metaObject()->className());
    }
  }
}

QMetaMethod Query::findOnObjectPropertyChangedMethod() {
  QMetaMethod result;
  for (int m = 0; m < this->metaObject()->methodCount(); m++) {
    QMetaMethod method = this->metaObject()->method(m);
#if QT_VERSION >= 0x050000
    if (QString(method.name()) == "onObjectPropertyChanged" && method.parameterCount() == 0) {
      result = method;
    }
#else
    if (QString(method.signature()) == "onObjectPropertyChanged()") {
      result = method;
    }
#endif
  }

  return result;
}

void Query::onObjectPropertyChanged() {
  QObject *sender = this->sender();
  if (sender) {
    QString senderPropertyName = getPropertyName(sender, senderSignalIndex());

    Mapping::ClassMapBase *classBase = ConfigurationMap::getMappedClass(sender->metaObject()->className());
    OneToMany *oneToMany = classBase->findOneToManyByPropertyName(senderPropertyName);
    if (oneToMany) {
      saveOneToMany(*sender, oneToMany);
    } else {
      OneToOne *oneToOne = classBase->findOneToOneByPropertyName(senderPropertyName);
      if (oneToOne) {
        saveOneToOne(*sender, oneToOne);
      }

      QString mainTableAlias = sqlBuilder->generateTableAlias();
      sqlBuilder->setClassBase(classBase);
      sqlBuilder->setTableAlias(mainTableAlias);
      sqlBuilder->setObject(sender);
      QSqlQuery query = sqlBuilder->updateOneColumnQuery(senderPropertyName);

      executeQuery(query);
    }
  }
}

QString Query::getPropertyName(QObject *sender, int senderSignalIndex) {
  QMetaMethod senderSignal = sender->metaObject()->method(senderSignalIndex);
  QMetaProperty senderProperty;
  const QMetaObject *senderMeta = sender->metaObject();
  for (int i = 0; i < senderMeta->propertyCount(); i++) {
    if (senderMeta->property(i).hasNotifySignal()) {
#if QT_VERSION >= 0x050000
      if (senderMeta->property(i).notifySignal() == senderSignal)
#else
      QMetaMethod signal = senderMeta->property(i).notifySignal();
      if (signal.signature() == senderSignal.signature())
#endif
      {
        senderProperty = senderMeta->property(i);
        break;
      }
    }
  }

  return QString(senderProperty.name());
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

bool Query::getAutoUpdate() const {
  return autoUpdate;
}

void Query::setAutoUpdate(bool value) {
  autoUpdate = value;
}

QMap<OneToOne *, QString> Query::getOneToOneAlias() const {
  return oneToOneAlias;
}

SqlBuilderBase *Query::getSqlBuilder() const {
  return sqlBuilder;
}

void Query::setSqlBuilder(SqlBuilderBase *value) {
  sqlBuilder = value;
  sqlBuilder->setDatabase(database);
}
}
}
