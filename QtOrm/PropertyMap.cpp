#include "PropertyMap.h"

namespace QtOrm {
namespace Mapping {

PropertyMap::PropertyMap() {
}

PropertyMap::PropertyMap(QString name) : name(name) {
}

PropertyMap::PropertyMap(QString name, QString column) : name(name), column(column) {
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
QString PropertyMap::getUpdateFunction() const {
  return updateFunction;
}

void PropertyMap::setUpdateFunction(const QString &value) {
  updateFunction = value;
}
}
}
