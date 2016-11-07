#ifndef E_H
#define E_H

#include <QObject>

#include "C.h"
#include "D.h"

class E : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(C *c READ getC WRITE setC NOTIFY cChanged)
  Q_PROPERTY(D *d READ getD WRITE setD NOTIFY dChanged)
public:
  Q_INVOKABLE explicit E(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  C *getC() const;
  void setC(C *value);

  D *getD() const;
  void setD(D *value);

signals:
  void cChanged();
  void dChanged();

private:
  long id = 0;
  C *c = nullptr;
  D *d = nullptr;
};

#endif // E_H
