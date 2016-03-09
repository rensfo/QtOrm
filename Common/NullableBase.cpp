#include "NullableBase.h"

NullableBase::NullableBase(QObject *parent) : QObject(parent), isNull(true), value(QVariant()) {
}

NullableBase::NullableBase(const NullableBase &value) {
    isNull = !value.hasValue();
    this->value = value.getVariant();
}

bool NullableBase::hasValue() const {
  return !isNull;
}

QVariant NullableBase::getVariant() const {
    return value;
}

void NullableBase::setVariant(const QVariant &value){
    this->value = value;
    if(this->value.isValid())
        isNull = false;
}

NullableBase &NullableBase::operator=(const NullableBase &nullableBase)
{
    isNull = !nullableBase.hasValue();
    if(isNull)
    {
        value = QVariant();
    }
    else
    {
        value = nullableBase.getVariant();
    }
    return *this;
}
