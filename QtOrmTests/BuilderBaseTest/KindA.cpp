#include "KindA.h"

KindA::KindA(QObject *parent) : QObject(parent)
{

}

long KindA::getId() const
{
  return id;
}

void KindA::setId(long value)
{
  id = value;
}

QString KindA::getCode() const
{
  return code;
}

void KindA::setCode(const QString &value)
{
  code = value;
  emit codeChanged();
}

QString KindA::getName() const
{
  return name;
}

void KindA::setName(const QString &value)
{
  name = value;
  emit nameChanged();
}

TypeA *KindA::getTypeA() const
{
  return typeA;
}

void KindA::setTypeA(TypeA *value)
{
  typeA = value;
  emit typeAChanged();
}