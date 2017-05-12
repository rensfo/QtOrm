#ifndef SUBCLASSS3_H
#define SUBCLASSS3_H

#include "SuperClassS.h"

#include <QSharedPointer>

class SubClassS3 : public SuperClassS
{
  Q_OBJECT
  Q_PROPERTY(QSharedPointer<SuperClassS> ref READ getRef WRITE setRef)
public:
  Q_INVOKABLE explicit SubClassS3(QObject* parent = nullptr);

  QSharedPointer<SuperClassS> getRef() const;
  void setRef(const QSharedPointer<SuperClassS>& value);

private:
  QSharedPointer<SuperClassS> ref;
};

#endif // SUBCLASSS3_H
