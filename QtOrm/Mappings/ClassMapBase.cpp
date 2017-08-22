#include "ClassMapBase.h"
#include "SubClassMap.h"

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

QMap<QString, QSharedPointer<PropertyMap>> ClassMapBase::getProperties() {
  return properties;
}

PropertyMap& ClassMapBase::setId(QString columnName) {
  return mapIdBase("", false).setColumn(columnName);
}

QString ClassMapBase::getClassName() const {
  return classMetaObject.className();
}

PropertyMap &ClassMapBase::mapId(QString propertyName) {
  return mapIdBase(propertyName, true);
}

PropertyMap &ClassMapBase::mapId(const QString &propertyName, const QString &columnName) {
  return mapId(propertyName).setColumn(columnName);
}

PropertyMap &ClassMapBase::mapDiscriminator(const QString &propertyName) {
  if (discriminatorProperty) {
    QString errorText = QString::fromUtf8("Discriminator field registred (%1.%2)").arg(getClassName()).arg(discriminatorProperty->getName());
    throw DiscriminatorPropertyAlreadyRegistredException(errorText);
  }

  discriminatorProperty = createProperty(propertyName);
  return discriminatorProperty->setColumn(propertyName).setIsDiscriminator(true);
}

PropertyMap &ClassMapBase::mapDiscriminator(const QString &propertyName, const QString &columnName) {
  return mapDiscriminator(propertyName).setColumn(columnName);
}

PropertyMap &ClassMapBase::map(QString propertyName) {
  return createProperty(propertyName)->setColumn(propertyName);
}

PropertyMap &ClassMapBase::map(QString propertyName, QString columnName) {
  return createProperty(propertyName)->setColumn(columnName);
}

QSharedPointer<PropertyMap> ClassMapBase::createProperty(QString propertyName, bool needCheckProperty) {
  if(needCheckProperty) {
    checkProperty(propertyName);
  }

  QSharedPointer<PropertyMap> propertyMap = QSharedPointer<PropertyMap>::create(propertyName);
  properties.insert(propertyName, propertyMap);

  return propertyMap;
}

PropertyMap&ClassMapBase::mapIdBase(QString propertyName, bool needCheckProperty) {
  if (idProperty) {
    QString errorText = QString::fromUtf8("Field id registred (%1.%2)").arg(getClassName()).arg(idProperty->getName());
    throw IdPropertyAlreadyRegistredException(errorText);
  }

  idProperty = createProperty(propertyName, needCheckProperty);
  return idProperty->setColumn(propertyName).setIsId(true).setNull(0);
}

QList<QSharedPointer<ClassMapBase> > ClassMapBase::getDerrivedClasses() const {
    return derrivedClasses;
}

void ClassMapBase::setDerrivedClasses(const QList<QSharedPointer<ClassMapBase> >&value) {
  derrivedClasses = value;
}

void ClassMapBase::appendDerrivedClass(QSharedPointer<ClassMapBase> value) {
  derrivedClasses.append(value);
}

void ClassMapBase::appendDerrivedClass(QList<QSharedPointer<ClassMapBase>>&values) {
  derrivedClasses.append(values);
}

void ClassMapBase::removeDerrivedClass(QSharedPointer<ClassMapBase> value) {
  derrivedClasses.removeAll(value);
}

bool ClassMapBase::isBaseClass() {
  return derrivedClasses.count();
}

InheritanceType ClassMapBase::getInheritanceType() {
  if(!derrivedClasses.count())
    return InheritanceType::None;

  SubClassMap* subClass = qobject_cast<SubClassMap*>(derrivedClasses.first().data());
  return subClass->getInheritanceType();
}

QMetaObject ClassMapBase::getClassMetaObject() const {
    return classMetaObject;
}

void ClassMapBase::setClassMetaObject(const QMetaObject&value) {
    classMetaObject = value;
}

QMetaObject ClassMapBase::getMetaObject() const {
  return classMetaObject;
}

void ClassMapBase::setMetaObject(const QMetaObject &metaObject) {
  this->classMetaObject = metaObject;
}

QSharedPointer<PropertyMap> ClassMapBase::getIdProperty() const {
  return idProperty;
}

QString ClassMapBase::getIdColumn() const {
  return getIdProperty()->getColumn();
}

QString ClassMapBase::getIdPropertyName() const {
  return getIdProperty()->getName();
}

QSharedPointer<PropertyMap> ClassMapBase::getProperty(const QString &property) {
  return properties.value(property);
}

QString ClassMapBase::getPropertyColumn(const QString &property) {
  if (!properties.contains(property))
    return QString();

  return getProperty(property)->getColumn();
}

OneToMany &ClassMapBase::addOneToMany(const QString &property) {
  checkRelationProperty(property);

  QSharedPointer<OneToMany> relation = QSharedPointer<OneToMany>::create();
  relation->setProperty(property);
  oneToManyRelations.append(relation);

  return *relation;
}

OneToOne &ClassMapBase::addOneToOne(const QString &property) {
  checkRelationProperty(property);

  QSharedPointer<OneToOne> relation = QSharedPointer<OneToOne>::create();
  relation->setProperty(property);
  oneToOneRelations.append(relation);

  return *relation;
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
  QString propertyType = getPropertyType(meta, prop);

  QRegExp rx("(\\w+<)*([\\w|:]+)[*]*>*");
  int pos = rx.indexIn(propertyType);
  QString result = propertyType;
  if (pos > -1) {
    result = rx.cap(2);
  }

  return result;
}

TypeKind ClassMapBase::getTypeKindOfProperty(QSharedPointer<QObject> &obj, const QString &prop) {
  return getTypeKindOfProperty(*obj->metaObject(), prop);
}

TypeKind ClassMapBase::getTypeKindOfProperty(const QMetaObject &meta, const QString &prop) {
  QString propertyType = getPropertyType(meta, prop);

  auto checker = [propertyType](const QString &pattern) -> bool {
    QRegExp rx(pattern);
    int pos = rx.indexIn(propertyType);
    return pos > -1;
  };

  std::map<QString, TypeKind> checkPatterns{{"^(QList<)*(.+)[*]>*$", TypeKind::Pointer},
                                            {"^(QList<)*QSharedPointer<.+>(>)*$", TypeKind::SharedPointer},
                                            {"^(QList<)*QWeakPointer<.+>(>)*$", TypeKind::WeakPointer}};

  for (auto pair : checkPatterns) {
    if (checker(pair.first)) {
      return pair.second;
    }
  }

  return TypeKind::Other;
}

bool ClassMapBase::containsProperty(const QString &propertyName) const {
  return properties.contains(propertyName);
}

bool ClassMapBase::isSubclass() {
  return false;
}

SubClassMap*ClassMapBase::toSubclass()
{
  return qobject_cast<SubClassMap*>(this);
}

QString ClassMapBase::getDiscriminatorPropertyName() const {
  return discriminatorProperty->getName();
}

QString ClassMapBase::getDiscriminatorColumn() const {
  return discriminatorProperty->getColumn();
}

QSharedPointer<PropertyMap> ClassMapBase::getDiscriminatorProperty() const {
  return discriminatorProperty;
}

QStringList ClassMapBase::getColumns() {
  QStringList columns;
  for (auto property : getProperties())
    columns << property->getColumn();

  return columns;
}

QVariant ClassMapBase::getDiscriminatorValue() const {
  return discrimanatorValue;
}

void ClassMapBase::setDiscrimanitorValue(const QVariant &value) {
  discrimanatorValue = value;
}

QString ClassMapBase::getPropertyType(const QMetaObject &meta, const QString &prop) {
  int propertyIndex = meta.indexOfProperty(prop.toStdString().data());
  QMetaProperty metaProperty = meta.property(propertyIndex);
  return metaProperty.typeName();
}

QList<QSharedPointer<OneToMany>> ClassMapBase::getOneToManyRelations() const {
  return oneToManyRelations;
}

QList<QSharedPointer<OneToOne>> ClassMapBase::getOneToOneRelations() const {
  return oneToOneRelations;
}
}
}
