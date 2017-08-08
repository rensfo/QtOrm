#include "SubClassMap.h"

namespace QtOrm {
namespace Mapping {

SubClassMap::SubClassMap(QObject *parent) : ClassMapBase(parent) {
}

QSharedPointer<ClassMapBase> SubClassMap::getSuperClass() const {
  return superClass;
}

void SubClassMap::setSuperClass(const QSharedPointer<ClassMapBase>&value) {
  superClass = value;
}

bool SubClassMap::isSubclass() {
  return true;
}

InheritanceType SubClassMap::getInheritanceType() const {
  return inheritanceType;
}

QSharedPointer<ClassMapBase> SubClassMap::getBaseClass() const {
  if(superClass->isSubclass()) {
    return superClass->toSubclass()->getBaseClass();
  }

  return superClass;
}

bool SubClassMap::isClassTableInheritance(const QSharedPointer<ClassMapBase>&classBase) {
  return classBase->isSubclass() && classBase->toSubclass()->getInheritanceType() == InheritanceType::ClassTable;
}

QMap<QString, QSharedPointer<PropertyMap>> SubClassMap::getAllProperties() {
  QMap<QString, QSharedPointer<PropertyMap>> allProperties = superClass->getProperties();
  for(auto prop : getProperties().toStdMap()){
    allProperties.insert(prop.first, prop.second);
  }

  return allProperties;
}

QString SubClassMap::getClassNameByProperty(const QString&property){
  if(getProperties().keys().contains(property)) {
    return getClassName();
  }

  if(superClass->isSubclass()) {
    return superClass->toSubclass()->getClassNameByProperty(property);
  } else {
    if(superClass->getProperties().keys().contains(property)){
      return superClass->getClassName();
    }
  }

  return QString::null;
}

void SubClassMap::setInheritanceType(const InheritanceType&value) {
    inheritanceType = value;
}

QString SubClassMap::getSuperClassName() const {
    return superClass->getClassName();
}

}
}
