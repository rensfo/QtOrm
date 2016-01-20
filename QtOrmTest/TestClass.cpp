#include "TestClass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{

}
int TestClass::getId() const
{
    return id;
}

void TestClass::setId(int value)
{
    id = value;
}
QString TestClass::getName() const
{
    return name;
}

void TestClass::setName(const QString &value)
{
    name = value;
}



