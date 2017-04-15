#include "SuperClassS.h"

SuperClassS::SuperClassS(QObject *parent) : QObject(parent)
{

}

long SuperClassS::getId() const
{
  return id;
}

void SuperClassS::setId(long value)
{
  id = value;
}

QString SuperClassS::getCode() const
{
  return code;
}

void SuperClassS::setCode(const QString &value)
{
  code = value;
  emit codeChanged();
}

int SuperClassS::getDiscriminator() const
{
  return discriminator;
}

void SuperClassS::setDiscriminator(int value)
{
  discriminator = value;
}
