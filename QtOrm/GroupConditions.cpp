#include "GroupConditions.h"

namespace QtOrm {
namespace Sql {

GroupConditions::GroupConditions(QObject *parent) : QObject(parent) {
}

GroupConditions::GroupConditions(const GroupConditions &group) : GroupConditions(nullptr) {
  setParent(group.parent());
  operation = group.getOperation();
  conditions = group.getConditions();
  groups = group.getGroups();
}
GroupOperation GroupConditions::getOperation() const {
  return operation;
}

void GroupConditions::setOperation(const GroupOperation &value) {
  operation = value;
}
QList<Condition *> GroupConditions::getConditions() const {
  return conditions;
}

void GroupConditions::setConditions(const QList<Condition *> &value) {
  conditions = value;
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

void GroupConditions::addCondition(const Condition &value) {
  conditions.append(new Condition(value));
}

void GroupConditions::addCondition(const QString &property, const Operation &operation, const QVariant &value) {
  Condition newFilter(this);
  newFilter.setPropertyName(property);
  newFilter.setOperation(operation);
  newFilter.setValue(value);

  addCondition(newFilter);
}

void GroupConditions::addConditionEqual(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::Equal, value);
}

void GroupConditions::removeFilter(Condition *value) {
  conditions.removeAll(value);
}

void GroupConditions::clearConditions() {
  conditions.clear();
}

void GroupConditions::clearGroups() {
  groups.clear();
}

void GroupConditions::clear() {
  clearConditions();
  clearGroups();
}

bool GroupConditions::isEmpty() const {
  return groups.isEmpty() && conditions.isEmpty();
}

GroupConditions &GroupConditions::operator=(const GroupConditions &group) {
  operation = group.getOperation();
  conditions = group.getConditions();
  groups = group.getGroups();

  return *this;
}
}
}
