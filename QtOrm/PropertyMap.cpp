#include "PropertyMap.h"

namespace QtOrm {
namespace Mapping {

PropertyMap::PropertyMap() : isId(false) {
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
}
}
