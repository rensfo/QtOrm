#include "A.h"

A::A(QObject *parent) : QObject(parent)
{

}

long A::getId() const
{
  return id;
}

void A::setId(long value)
{
  id = value;
}

QString A::getCode() const
{
  return code;
}

void A::setCode(const QString &value)
{
  code = value;
  emit codeChanged();
}

QList<B *> A::getChild() const
{
  return child;
}

void A::setChild(const QList<B *> &value)
{
  child = value;
}

KindA *A::getKindA() const
{
  return kindA;
}

void A::setKindA(KindA *value)
{
  kindA = value;
  emit kindAChanged();
}
