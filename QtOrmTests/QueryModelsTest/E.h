#ifndef E_H
#define E_H

#include <QObject>

#include "C.h"
#include "D.h"

class E : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QSharedPointer<C> c READ getC WRITE setC NOTIFY cChanged)
  Q_PROPERTY(QSharedPointer<D> d READ getD WRITE setD NOTIFY dChanged)
public:
  Q_INVOKABLE explicit E(QObject *parent = nullptr);
  ~E();

  long getId() const;
  void setId(long value);

  QSharedPointer<C> getC() const;
  void setC(QSharedPointer<C> value);

  QSharedPointer<D> getD() const;
  void setD(QSharedPointer<D> value);

signals:
  void cChanged();
  void dChanged();

private:
  long id = 0;
  QSharedPointer<C> c;
  QSharedPointer<D> d;
};

#endif // E_H
