#include "B.h"

B::B(QObject *parent) : QObject(parent)
{

}

long B::getId() const
{
    return id;
}

void B::setId(long value)
{
    id = value;
}

long B::getIdA() const
{
    return idA;
}

void B::setIdA(long value)
{
    idA = value;
}

QString B::getCode() const
{
    return code;
}

void B::setCode(const QString &value)
{
    code = value;
}
