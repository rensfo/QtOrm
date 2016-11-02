#include "OneToMany.h"

namespace QtOrm {
namespace Mapping {

OneToMany::OneToMany(QObject *parent) : Relation(parent) {}
QString OneToMany::getProperty() const { return property; }

OneToMany &OneToMany::setProperty(const QString &property) {
  this->property = property;
  return *this;
}
QString OneToMany::getRefClass() const { return refClass; }

OneToMany &OneToMany::setRefClass(const QString &refClass) {
  this->refClass = refClass;
  return *this;
}
QString OneToMany::getRefProperty() const { return refProperty; }

OneToMany &OneToMany::setRefProperty(const QString &refProperty) {
  this->refProperty = refProperty;
  return *this;
}

OneToMany &OneToMany::setSaveCascade(bool value) {
  saveCascade = value;
  return *this;
}
}
}
