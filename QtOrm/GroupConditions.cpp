#include "GroupConditions.h"

GroupConditions::GroupConditions(QObject *parent) : QObject(parent) {
}

GroupConditions::GroupConditions(const GroupConditions &group) : GroupConditions(nullptr) {
  setParent(group.parent());
  operation = group.getOperation();
  filters = group.getFilters();
  groups = group.getGroups();
}
GroupOperation GroupConditions::getOperation() const {
  return operation;
}

void GroupConditions::setOperation(const GroupOperation &value) {
  operation = value;
}
QList<Condition *> GroupConditions::getFilters() const {
  return filters;
}

void GroupConditions::setFilters(const QList<Condition *> &value) {
  filters = value;
}
QList<GroupConditions *> GroupConditions::getGroups() const {
  return groups;
}

void GroupConditions::setGroups(const QList<GroupConditions *> &value) {
  groups = value;
}

void GroupConditions::addGroup(const GroupConditions &value) {
  groups.append(new GroupConditions(value));
}

void GroupConditions::addFilter(const Condition &value) {
  filters.append(new Condition(value));
}

void GroupConditions::addFilter(const QString &fieldName, const Operation &operation, const QVariant &value) {
  Condition newFilter(this);
  newFilter.setFieldName(fieldName);
  newFilter.setOperation(operation);
  newFilter.setValue(value);

  addFilter(newFilter);
}

void GroupConditions::addFilterEqual(const QString &fieldName, const QVariant &value) {
  addFilter(fieldName, Operation::Equal, value);
}

void GroupConditions::removeFilter(Condition *value) {
  filters.removeAll(value);
}

void GroupConditions::clearFilters() {
  filters.clear();
}

void GroupConditions::clearGroups() {
  groups.clear();
}

void GroupConditions::clear() {
  clearFilters();
  clearGroups();
}

bool GroupConditions::isEmpty() const {
  return groups.isEmpty() && filters.isEmpty();
}

GroupConditions &GroupConditions::operator=(const GroupConditions &group) {
  operation = group.getOperation();
  filters = group.getFilters();
  groups = group.getGroups();

  return *this;
}
