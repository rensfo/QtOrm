#include "D.h"

D::D(QObject *parent) : QObject(parent)
{

}

KindA *D::getKindA() const
{
  return kindA;
}

void D::setKindA(KindA *value)
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
