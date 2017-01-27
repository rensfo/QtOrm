#include "Condition.h"

namespace QtOrm {
namespace Sql {

Condition::Condition() {
}

Condition::Condition(const QString &property, Operation operation, const QVariant &value)
    : property(property), operation(operation), values({value}) {
}

Condition::Condition(const QString &property, Operation operation, const QVariantList &values)
    : property(property), operation(operation), values(values) {
}

Condition::Condition(const Condition &other) : Condition() {
  values = other.getValues();
  operation = other.getOperation();
  property = other.getProperty();
}

Condition::~Condition() {
}

QVariantList Condition::getValues() const {
  return values;
}

void Condition::setValues(const QList<QVariant> &value) {
  values = value;
}

void Condition::setValue(const QVariant &value) {
  values.append(value);
}

void Condition::clearValues() {
  values.clear();
}

QString Condition::getProperty() const {
  return property;
}

void Condition::setProperty(const QString &value) {
  property = value;
}

Operation Condition::getOperation() const {
  return operation;
}

void Condition::setOperation(const Operation &value) {
  operation = value;
}
}
}
