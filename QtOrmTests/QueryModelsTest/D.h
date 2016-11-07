#ifndef D_H
#define D_H

#include <QObject>

#include "KindA.h"

class D : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(KindA *kindA READ getKindA WRITE setKindA NOTIFY kindAChanged)
public:
  Q_INVOKABLE explicit D(QObject *parent = nullptr);

  KindA *getKindA() const;
  void setKindA(KindA *value);

  long getId() const;
  void setId(long value);

signals:
  void kindAChanged();

private:
  long id = 0;
  KindA *kindA = nullptr;
};

#endif // D_H
