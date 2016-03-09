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
QList<Filter *> Group::getFilters() const {
  return filters;
}

void Group::setFilters(const QList<Filter *> &value) {
  filters = value;
}
QList<Group *> Group::getGroups() const {
  return groups;
}

void Group::setGroups(const QList<Group *> &value) {
  groups = value;
}

void Group::addGroup(Group *const &value) {
  groups.append(value);
}

void Group::addFilter(Filter *const &value) {
  filters.append(value);
}

void Group::removeFilter(Filter *value) {
  filters.removeAll(value);
}

bool Group::isEmpty() const {
  return groups.isEmpty() && filters.isEmpty();
}
