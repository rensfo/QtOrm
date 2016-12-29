#include "GroupConditions.h"
#include "Conditions/ConditionFactory.h"

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

//void GroupConditions::addCondition(const Condition &value) {
//  conditions.append(QSharedPointer<Condition>::create(value));
//}

void GroupConditions::addCondition(const QSharedPointer<Condition> &value) {
  conditions.append(value);
}

void GroupConditions::addCondition(const QString &property, const Operation &operation, const QVariant &value) {
  QSharedPointer<Condition> newFilter = ConditionFactory::create(property, operation, value);

  addCondition(newFilter);
}

void GroupConditions::addCondition(const QString &property, const Operation &operation, const QVariantList &values) {
  QSharedPointer<Condition> newFilter = ConditionFactory::create(property, operation, values);

  addCondition(newFilter);
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
