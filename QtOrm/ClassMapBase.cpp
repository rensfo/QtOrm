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

const QMap<QString, QSharedPointer<PropertyMap>> ClassMapBase::getProperties() const {
  return properties;
}

QString ClassMapBase::getClassName() const {
  return classMetaObject.className();
}

PropertyMap &ClassMapBase::id(QString propertyName) {
  if (!idProperty.isEmpty()) {
    QString errorText = QString::fromUtf8("Field id registred (%1.%2)").arg(getClassName()).arg(idProperty);
    throw IdPropertyAlreadyRegistredException(errorText);
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

  QSharedPointer<PropertyMap> propertyMap = QSharedPointer<PropertyMap>::create(propertyName);
  properties.insert(propertyName, propertyMap);

  return *propertyMap;
}

QMetaObject ClassMapBase::getMetaObject() const {
  return classMetaObject;
}

void ClassMapBase::setMetaObject(const QMetaObject &metaObject) {
  this->classMetaObject = metaObject;
}

QSharedPointer<PropertyMap> ClassMapBase::getIdProperty() const {
  return properties.value(idProperty);
}

QSharedPointer<PropertyMap> ClassMapBase::getProperty(const QString &property) {
  return properties.value(property);
}

OneToMany &ClassMapBase::oneToMany(const QString &property) {
  checkToExistProperty(property);

  QSharedPointer<OneToMany> relation = QSharedPointer<OneToMany>::create();
  relation->setProperty(property);
  oneToManyRelations.append(relation);

  return *relation;
}

OneToOne &ClassMapBase::oneToOne(const QString &property) {
  checkToExistProperty(property);

  QSharedPointer<OneToOne> relation = QSharedPointer<OneToOne>::create();
  relation->setProperty(property);
  oneToOneRelations.append(relation);

  return *relation;
}

void ClassMapBase::checkToExistProperty(const QString &property) {
  int propertyIndex = classMetaObject.indexOfProperty(property.toStdString().data());
  if (propertyIndex == -1) {
    QString message =
        QString::fromUtf8("Property %2 in class %1 not found").arg(classMetaObject.className()).arg(property);
    throw PropertyNotFoundException(message);
  }
}

QSharedPointer<OneToOne> ClassMapBase::findOneToOneByPropertyName(const QString &propertyName) {
  for (auto oneToOne : oneToOneRelations) {
    if (oneToOne->getProperty() == propertyName)
      return oneToOne;
  }

  return QSharedPointer<OneToOne>();
}

QSharedPointer<OneToMany> ClassMapBase::findOneToManyByPropertyName(const QString &propertyName) {
  for (auto oneToMany : oneToManyRelations) {
    if (oneToMany->getProperty() == propertyName)
      return oneToMany;
  }

  return QSharedPointer<OneToMany>();
}

QString ClassMapBase::getTypeNameOfProperty(QSharedPointer<QObject> obj, const QString &prop) {
  return getTypeNameOfProperty(*obj->metaObject(), prop);
}

QString ClassMapBase::getTypeNameOfProperty(const QMetaObject &meta, const QString &prop) {
  int propertyIndex = meta.indexOfProperty(prop.toStdString().data());
  QMetaProperty metaProperty = meta.property(propertyIndex);
  QString refClass = metaProperty.typeName();

  QString sharedPointerClassName = "QSharedPointer<";

  if (refClass.contains(sharedPointerClassName)) {
    int begin = sharedPointerClassName.size();
    int length = refClass.size() - begin - 1;
    refClass = refClass.mid(begin, length);
  }

  if (refClass.right(1) == "*") {
    refClass = refClass.left(refClass.size() - 1);
  }

  return refClass;
}

bool ClassMapBase::propertiesContains(const QString &propertyName) {
  return properties.contains(propertyName);
}

QList<QSharedPointer<OneToMany>> ClassMapBase::getOneToManyRelations() const {
  return oneToManyRelations;
}

QList<QSharedPointer<OneToOne>> ClassMapBase::getOneToOneRelations() const {
  return oneToOneRelations;
}
}
}
