#include "OneToMany.h"

namespace QtOrm {
namespace Mapping {

OneToMany::OneToMany(QObject *parent) : Relation(parent) {
}

OneToMany &OneToMany::setProperty(const QString &property) {
  this->property = property;
  return *this;
}

QString OneToMany::getColumn() const {
  return column;
}

OneToMany &OneToMany::setColumn(const QString &column) {
  this->column = column;
  return *this;
}

OneToMany &OneToMany::setSaveCascade(bool value) {
  saveCascade = value;
  return *this;
}

OneToMany&OneToMany::setDeleteCascade(bool value)
{
  deleteCascade = value;
  return *this;
}

OneToMany &OneToMany::setOrderBy(const QString &property, Sort sort) {
  return setOrderBy({{property, sort}});
}

OneToMany &OneToMany::setOrderBy(const QList<OrderColumn> &orderBy) {
  this->orderBy = orderBy;
  return *this;
}

QList<OrderColumn> OneToMany::getOrderBy() {
  return orderBy;
}

OneToMany &OneToMany::setCondition(const Sql::Condition &condition) {
  group.addCondition(condition);
  return *this;
}

Sql::GroupConditions &OneToMany::getCondition() {
  return group;
}
}
}
