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

void SubClassMap::setInheritanceType(const InheritanceType&value) {
    inheritanceType = value;
}

QString SubClassMap::getSuperClassName() const {
    return superClass->getClassName();
}

}
}
