#include "SubClassS2.h"

SubClassS2::SubClassS2(QObject *parent) : SuperClassS(parent)
{
  setDiscriminator(2);
}

QString SubClassS2::getStrVal() const
{
  return strVal;
}

void SubClassS2::setStrVal(const QString &value)
{
  strVal = value;
  emit strValChanged();
}
