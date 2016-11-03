#include "ClassMapBase.h"

#include <QMetaProperty>

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
  return classMetaObject.className();
}

PropertyMap &ClassMapBase::id(QString propertyName) {
  if (!idProperty.isEmpty()) {
    QString errorText = QString::fromUtf8("Field id registred (%1.%2)").arg(getClassName()).arg(idProperty);
    throw Exception(ErrorCode::IdPropertyAlreadyRegistred, errorText);
  }

  idProperty = propertyName;
  return createProperty(propertyName).setColumn(propertyName).setIsId(true);
}

PropertyMap &ClassMapBase::map(QString propertyName) {
  return createProperty(propertyName).setColumn(propertyName);
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
  return classMetaObject;
}

void ClassMapBase::setMetaObject(const QMetaObject &metaObject) {
  this->classMetaObject = metaObject;
}

PropertyMap &ClassMapBase::getIdProperty() const {
  return *(properties.value(idProperty));
}

PropertyMap &ClassMapBase::getProperty(const QString &property) {
  return *(properties.value(property));
}

OneToMany &ClassMapBase::oneToMany(const QString &property) {
  checkToExistProperty(property);

  OneToMany *relation = new OneToMany();
  relation->setProperty(property);
  oneToManyRelations.append(relation);

  return *relation;
}

OneToOne &ClassMapBase::oneToOne(const QString &property) {
  checkToExistProperty(property);

  OneToOne *relation = new OneToOne();
  relation->setProperty(property);
  oneToOneRelations.append(relation);

  return *relation;
}

void ClassMapBase::checkToExistProperty(const QString &property) {
  int propertyIndex = classMetaObject.indexOfProperty(property.toStdString().data());
  if (propertyIndex == -1) {
    QString message =
        QString::fromUtf8("Property %2 in class %1 not found").arg(classMetaObject.className()).arg(property);
    throw Exception(ErrorCode::PropertyNotFound, message);
  }
}

OneToOne *ClassMapBase::findOneToOneByPropertyName(const QString &propertyName) {
  for (OneToOne *oneToOne : oneToOneRelations) {
    if (oneToOne->getProperty() == propertyName)
      return oneToOne;
  }

  return nullptr;
}

OneToMany *ClassMapBase::findOneToManyByPropertyName(const QString &propertyName) {
  for (OneToMany *oneToMany : oneToManyRelations) {
    if (oneToMany->getProperty() == propertyName)
      return oneToMany;
  }

  return nullptr;
}

QString ClassMapBase::getTypeNameOfProperty(const QObject &obj, const QString &prop) {
  return getTypeNameOfProperty(*obj.metaObject(), prop);
}

QString ClassMapBase::getTypeNameOfProperty(const QMetaObject &meta, const QString &prop) {
  int propertyIndex = meta.indexOfProperty(prop.toStdString().data());
  QMetaProperty metaProperty = meta.property(propertyIndex);
  QString refClass = metaProperty.typeName();
  if (refClass.right(1) == "*")
    refClass = refClass.left(refClass.size() - 1);

  return refClass;
}

QList<OneToMany *> ClassMapBase::getOneToManyRelations() const {
  return oneToManyRelations;
}

QList<OneToOne *> ClassMapBase::getOneToOneRelations() const {
  return oneToOneRelations;
}
}
}
