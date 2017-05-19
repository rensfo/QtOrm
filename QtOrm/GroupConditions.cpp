#include "GroupConditions.h"

#include "Conditions/Condition.h"

namespace QtOrm {
namespace Sql {

GroupConditions::GroupConditions(QObject *parent) : QObject(parent) {
}

GroupConditions::GroupConditions() : GroupConditions(nullptr) {
}

GroupConditions::GroupConditions(const GroupConditions &other) : GroupConditions() {
  setParent(other.parent());
  operation = other.getOperation();
  conditions = other.getConditions();
  groups = other.getGroups();
}

GroupConditions::GroupConditions(GroupConditions &&other) : GroupConditions() {
  operation = other.getOperation();
  conditions = other.getConditions();
  groups = other.getGroups();
  other.clear();
}
GroupOperation GroupConditions::getOperation() const {
  return operation;
}

void GroupConditions::setOperation(const GroupOperation &value) {
  operation = value;
}
QList<QSharedPointer<Condition>> GroupConditions::getConditions() const {
  return conditions;
}

void GroupConditions::setConditions(const QList<QSharedPointer<Condition>> &value) {
  conditions = value;
}
QList<QSharedPointer<GroupConditions>> GroupConditions::getGroups() const {
  return groups;
}

void GroupConditions::setGroups(const QList<QSharedPointer<GroupConditions>> &value) {
  groups = value;
}

void GroupConditions::addGroup(const GroupConditions &value) {
  groups.append(QSharedPointer<GroupConditions>::create(value));
}

void GroupConditions::addCondition(const Condition &value) {
  conditions.append(QSharedPointer<Condition>::create(value));
}

void GroupConditions::addCondition(const QSharedPointer<Condition> &value) {
  conditions.append(value);
}

void GroupConditions::addCondition(const QString &property, const Operation &operation, const QVariant &value) {
  addCondition(QSharedPointer<Condition>::create(property, operation, value));
}

void GroupConditions::addCondition(const QString &property, const Operation &operation, const QVariantList &values) {
  addCondition(QSharedPointer<Condition>::create(property, operation, values));
}

GroupConditions &GroupConditions::operator&&(const Condition &other) {
  if (operation == GroupOperation::And) {
    addCondition(other);
  } else {
    GroupConditions newGroup(std::move(*this));
    operation = GroupOperation::And;
    *this &&newGroup &&other;
  }

  return *this;
}

GroupConditions &GroupConditions::operator||(const Condition &other) {
  if (operation == GroupOperation::Or) {
    addCondition(other);
  } else {
    GroupConditions newGroup(std::move(*this));
    operation = GroupOperation::Or;
    *this || newGroup  || other;
  }

  return *this;
}

GroupConditions &GroupConditions::operator&&(const GroupConditions &other) {
  if (operation == GroupOperation::And) {
    addGroup(other);
  } else {
    GroupConditions newGroup(std::move(*this));
    operation = GroupOperation::And;
    *this &&newGroup &&other;
  }

  return *this;
}

GroupConditions &GroupConditions::operator||(const GroupConditions &other) {
  if (operation == GroupOperation::Or) {
    addGroup(other);
  } else {
    GroupConditions newGroup(std::move(*this));
    operation = GroupOperation::Or;
    *this || newGroup  || other;
  }

  return *this;
}

void GroupConditions::addEqual(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::Equal, value);
}

void GroupConditions::addNotEqual(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::NotEqual, value);
}

void GroupConditions::addLike(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::Like, value);
}

void GroupConditions::addBetween(const QString &fieldName, const QVariant &value1, const QVariant &value2) {
  QVariantList values = {value1, value2};
  addCondition(fieldName, Operation::Between, values);
}

void GroupConditions::addIn(const QString &fieldName, const QVariantList &values) {
  addCondition(fieldName, Operation::In, values);
}

void GroupConditions::addGreater(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::Greater, value);
}

void GroupConditions::addGreaterOrEqual(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::GreaterOrEqual, value);
}

void GroupConditions::addLess(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::Less, value);
}

void GroupConditions::addLessOrEqual(const QString &fieldName, const QVariant &value) {
  addCondition(fieldName, Operation::LessOrEqual, value);
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
