#include "D.h"

D::D(QObject *parent) : QObject(parent)
{

}

QSharedPointer<KindA> D::getKindA() const
{
  return kindA;
}

void D::setKindA(QSharedPointer<KindA> value)
{
  kindA = value;
}

long D::getId() const
{
  return id;
}

void D::setId(long value)
{
  id = value;
}
