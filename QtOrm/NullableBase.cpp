#include "NullableBase.h"

NullableBase::NullableBase(QObject *parent) : QObjectData(parent)
{

}

bool NullableBase::hasValue() const
{
    return !isNull;
}

