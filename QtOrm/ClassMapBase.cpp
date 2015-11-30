#include "ClassMapBase.h"
#include <QException>

namespace QtOrm {
namespace Mapping {

ClassMapBase::ClassMapBase(QObject *parent) : QObject(parent) {
}

QString ClassMapBase::getTable() const {
  return table;
}

void ClassMapBase::setTable(const QString &table) {
  this->table = table;
}

const QMap<QString, PropertyMap *> ClassMapBase::getProperties() const {
  return properties;
}

QString ClassMapBase::getClassName() const {
  return metaObject.className();
}

PropertyMap &ClassMapBase::id(QString propertyName) {
  if (!idProperty.isEmpty()) {
    QString errorText = QString("При связывании поля '%1' класса '%2' произошла исключительная "
                                "ситуация - id уже зарезервировано за '%3'")
                            .arg(propertyName)
                            .arg(getClassName())
                            .arg(idProperty);
    throw new Exception(errorText);
  }

  idProperty = propertyName;
  return createProperty(propertyName).setIsId(true);
}

PropertyMap &ClassMapBase::map(QString propertyName) {
  return createProperty(propertyName);
}

PropertyMap &ClassMapBase::map(QString propertyName, QString columnName) {
  return createProperty(propertyName).setColumn(columnName);
}

PropertyMap &ClassMapBase::createProperty(QString propertyName) {
  checkToExistProperty(propertyName);

  PropertyMap *propertyMap = new PropertyMap(propertyName);
  properties.insert(propertyName, propertyMap);

  return *propertyMap;
}

QMetaObject ClassMapBase::getMetaObject() const {
  return metaObject;
}

void ClassMapBase::setMetaObject(const QMetaObject &metaObject) {
  this->metaObject = metaObject;
}

PropertyMap &ClassMapBase::getIdProperty() {
  return *(properties.value(idProperty));
}

PropertyMap &ClassMapBase::getProperty(const QString &property) {
  return *(properties.value(property));
}

OneToMany &ClassMapBase::oneToMany(const QString &property) {
  checkToExistProperty(property);

  OneToMany *relation = new OneToMany();
  relation->setProperty(property);
  oneToManyRelations.insert(property, relation);

  return *relation;
}

OneToOne &ClassMapBase::oneToOne(const QString &property) {
  checkToExistProperty(property);

  OneToOne *relation = new OneToOne();
  relation->setProperty(property);
  oneToOneRelations.insert(property, relation);

  return *relation;
}

void ClassMapBase::checkToExistProperty(const QString &property) {
  int propertyIndex = metaObject.indexOfProperty(property.toStdString().data());
  if (propertyIndex == -1) {
    QString message = QString("В классе %1 свойство %2 не найдено.").arg(metaObject.className()).arg(property);
    throw new Exception(message);
  }
}
QString ClassMapBase::getDeleteFunction() const {
  return deleteFunction;
}

void ClassMapBase::setDeleteFunction(const QString &value) {
  deleteFunction = value;
}

QString ClassMapBase::getInsertFunction() const {
  return insertFunction;
}

void ClassMapBase::setInsertFunction(const QString &value) {
  insertFunction = value;
}

QString ClassMapBase::getContext() const {
  return context;
}

void ClassMapBase::setContext(const QString &value) {
  context = value;
}

QMap<QString, OneToMany *> ClassMapBase::getOneToManyRelations() const {
  return oneToManyRelations;
}

QMap<QString, OneToOne *> ClassMapBase::getOneToOneRelations() const {
  return oneToOneRelations;
}
}
}
