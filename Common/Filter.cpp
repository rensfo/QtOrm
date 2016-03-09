#include "Filter.h"

Filter::Filter(QObject *parent) : QObject(parent)
{
}

Filter::Filter(const Filter &filter) : Filter(nullptr)
{
    fieldName = filter.getFieldName();
    operation = filter.getOperation();
    values = filter.getValues();
}
QString Filter::getFieldName() const
{
    return fieldName;
}

void Filter::setFieldName(const QString &value)
{
    fieldName = value;
}
Operation Filter::getOperation() const
{
    return operation;
}

void Filter::setOperation(const Operation &value)
{
    operation = value;
}
QList<QVariant> Filter::getValues() const
{
    return values;
}

void Filter::setValues(const QList<QVariant> &value)
{
    values = value;
}

void Filter::clearValues()
{
    values.clear();
}

void Filter::addValue(const QVariant &value)
{
    values.append(value);
}




