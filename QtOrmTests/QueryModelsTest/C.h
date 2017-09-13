#ifndef C_H
#define C_H

#include <QObject>

#include "KindA.h"

class C : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QSharedPointer<KindA> kindA READ getKindA WRITE setKindA NOTIFY kindAChanged)
public:
  Q_INVOKABLE explicit C(QObject *parent = nullptr);
  ~C();

  long getId() const;
  void setId(long value);

  QSharedPointer<KindA> getKindA() const;
  void setKindA(QSharedPointer<KindA> value);

signals:
  void kindAChanged();

private:
  long id = 0;
  QSharedPointer<KindA> kindA;
};

#endif // C_H
