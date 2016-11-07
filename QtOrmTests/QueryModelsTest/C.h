#ifndef C_H
#define C_H

#include <QObject>

#include "KindA.h"

class C : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(KindA *kindA READ getKindA WRITE setKindA NOTIFY kindAChanged)
public:
  Q_INVOKABLE explicit C(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  KindA *getKindA() const;
  void setKindA(KindA *value);

signals:
  void kindAChanged();

private:
  long id = 0;
  KindA *kindA = nullptr;
};

#endif // C_H
