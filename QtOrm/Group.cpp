#include "Group.h"

Group::Group(QObject *parent) : QObject(parent) {
}

Group::Group(const Group &group) : Group(nullptr) {
  setParent(group.parent());
  operation = group.getOperation();
  filters = group.getFilters();
  groups = group.getGroups();
}
GroupOperation Group::getOperation() const {
  return operation;
}

void Group::setOperation(const GroupOperation &value) {
  operation = value;
}
QList<Condition *> Group::getFilters() const {
  return filters;
}

void Group::setFilters(const QList<Condition *> &value) {
  filters = value;
}
QList<Group *> Group::getGroups() const {
  return groups;
}

void Group::setGroups(const QList<Group *> &value) {
  groups = value;
}

void Group::addGroup(const Group &value) {
  groups.append(new Group(value));
}

void Group::addFilter(const Condition &value) {
  filters.append(new Condition(value));
}

void Group::addFilter(const QString &fieldName, const Operation &operation, const QVariant &value) {
  Condition newFilter(this);
  newFilter.setFieldName(fieldName);
  newFilter.setOperation(operation);
  newFilter.setValue(value);

  addFilter(newFilter);
}

void Group::addFilterEqual(const QString &fieldName, const QVariant &value) {
  addFilter(fieldName, Operation::Equal, value);
}

void Group::removeFilter(Condition *value) {
  filters.removeAll(value);
}

void Group::clearFilters() {
  filters.clear();
}

void Group::clearGroups() {
  groups.clear();
}

void Group::clear() {
  clearFilters();
  clearGroups();
}

bool Group::isEmpty() const {
  return groups.isEmpty() && filters.isEmpty();
}

Group &Group::operator=(const Group &group) {
  operation = group.getOperation();
  filters = group.getFilters();
  groups = group.getGroups();

  return *this;
}
