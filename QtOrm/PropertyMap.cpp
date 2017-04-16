#include "PropertyMap.h"

namespace QtOrm {
namespace Mapping {

PropertyMap::PropertyMap() {
}

PropertyMap::PropertyMap(QString name) : PropertyMap() {
  this->name = name;
}

PropertyMap::PropertyMap(QString name, QString column) : PropertyMap() {
  this->name = name;
  this->column = column;
}

PropertyMap &PropertyMap::setName(QString name) {
  this->name = name;
  return *this;
}

QString PropertyMap::getName() const {
  return name;
}

PropertyMap &PropertyMap::setColumn(QString column) {
  this->column = column;
  return *this;
}

QString PropertyMap::getColumn() const {
  return column;
}

PropertyMap &PropertyMap::setIsId(bool isId) {
  this->isId = isId;
  return *this;
}

bool PropertyMap::getIsId() {
  return isId;
}

bool PropertyMap::getIsDiscriminator() const {
  return isDiscriminator;
}

PropertyMap &PropertyMap::setIsDiscriminator(bool value) {
  isDiscriminator = value;
  return *this;
}

QVariant PropertyMap::getNull() const {
  return null;
}

PropertyMap &PropertyMap::setNull(const QVariant &value) {
  null = value;
  return *this;
}
}
}
