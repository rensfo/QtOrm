#include "Filter.h"

extern QMap<Operation, QString> OperationSimbols;

Filter::Filter(QObject *parent) : QObject(parent) {}

Filter::Filter(const QString &fieldName, const Operation &operation,
               const QVariant &value, QObject *parent)
    : Filter(parent) {
  this->fieldName = fieldName;
  this->operation = operation;
  this->values.append(value);
}

Filter::Filter(const Filter &filter) : Filter(nullptr) {
  fieldName = filter.getFieldName();
  operation = filter.getOperation();
  values = filter.getValues();
}

QString Filter::getFieldName() const { return fieldName; }

void Filter::setFieldName(const QString &value) { fieldName = value; }

Operation Filter::getOperation() const { return operation; }

void Filter::setOperation(const Operation &value) { operation = value; }

QList<QVariant> Filter::getValues() const { return values; }

void Filter::setValues(const QList<QVariant> &value) { values = value; }

void Filter::setValue(const QVariant &value) { values.append(value); }

void Filter::clearValues() { values.clear(); }

void Filter::addValue(const QVariant &value) { values.append(value); }
