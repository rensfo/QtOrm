#include "Condition.h"

namespace QtOrm {
namespace Sql {
extern QMap<Operation, QString> OperationStrings;

Condition::Condition(QObject *parent) : QObject(parent) {
}

Condition::Condition(const QString &fieldName, const Operation &operation, const QVariant &value, QObject *parent)
    : Condition(parent) {
  this->propertyName = fieldName;
  this->operation = operation;
  this->values.append(value);
}

Condition::Condition(const Condition &filter) : Condition(nullptr) {
  propertyName = filter.getPropertyName();
  operation = filter.getOperation();
  values = filter.getValues();
  column = filter.getColumn();
}

QString Condition::getPropertyName() const {
  return propertyName;
}

void Condition::setPropertyName(const QString &value) {
  propertyName = value;
}

Operation Condition::getOperation() const {
  return operation;
}

void Condition::setOperation(const Operation &value) {
  operation = value;
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

void Condition::addValue(const QVariant &value) {
  values.append(value);
}

QString Condition::getColumn() const {
  return column;
}

void Condition::setColumn(const QString &value) {
  column = value;
}
}
}
