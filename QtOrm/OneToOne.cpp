#include "OneToOne.h"

namespace QtOrm {
namespace Mapping {

OneToOne::OneToOne(QObject *parent) : Relation(parent) {
}

QString OneToOne::getProperty() const {
  return property;
}

OneToOne &OneToOne::setProperty(const QString &property) {
  this->property = property;
  return *this;
}

QString OneToOne::getTableColumn() const {
  return tableColumn;
}

OneToOne &OneToOne::setTableColumn(const QString &value) {
  tableColumn = value;
  return *this;
}

OneToOne &OneToOne::setSaveCascade(bool value) {
  saveCascade = value;
  return *this;
}
}
}
