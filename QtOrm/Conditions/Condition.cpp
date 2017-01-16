#include "Condition.h"

namespace QtOrm {
namespace Sql {
extern QMap<Operation, QString> OperationStrings;

Condition::Condition() {
}

Condition::Condition(const QString &property, const QVariant &value) : Condition() {
  this->property = property;
  this->values.append(value);
}

Condition::Condition(const QString &property, const QVariantList &values) : Condition() {
  this->property = property;
  this->values = values;
}

Condition::Condition(const Condition &other) : Condition() {
//  operationSymbol = other.getOperationSymbol();
  values = other.getValues();
//  column = other.getColumn();
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

//QString Condition::getColumn() const {
//  return column;
//}

//void Condition::setColumn(const QString &value) {
//  column = value;
//}

QString Condition::getProperty() const
{
  return property;
}

void Condition::setProperty(const QString &value)
{
  property = value;
}
}
}
