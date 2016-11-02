#ifndef A_H
#define A_H

#include <QObject>

#include "B.h"
#include "KindA.h"

class A : public QObject
{
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QString code READ getCode WRITE setCode NOTIFY codeChanged)
  Q_PROPERTY(KindA *kindA READ getKindA WRITE setKindA NOTIFY kindAChanged)
  Q_PROPERTY(QList<B *> child READ getChild WRITE setChild)
public:
  Q_INVOKABLE explicit A(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  QString getCode() const;
  void setCode(const QString &value);

  KindA *getKindA() const;
  void setKindA(KindA *value);

  QList<B *> getChild() const;
  void setChild(const QList<B *> &value);

signals:
  void codeChanged();
  void kindAChanged();

protected:
  long id;
  QString code;
  KindA *kindA;
  QList<B *> child;
};

#endif // A_H
