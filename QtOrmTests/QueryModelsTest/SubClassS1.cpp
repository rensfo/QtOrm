#include "SubClassS1.h"

SubClassS1::SubClassS1(QObject* parent) : SuperClassS(parent)
{
  setDiscriminator(1);
}

int SubClassS1::getIntVal() const
{
  return intVal;
}

void SubClassS1::setIntVal(int value)
{
  intVal = value;
  emit intValChanged();
}
