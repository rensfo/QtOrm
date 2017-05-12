#include "SubClassS3.h"

SubClassS3::SubClassS3(QObject* parent) : SuperClassS(parent)
{
}

QSharedPointer<SuperClassS> SubClassS3::getRef() const
{
  return ref;
}

void SubClassS3::setRef(const QSharedPointer<SuperClassS>& value)
{
  ref = value;
}
