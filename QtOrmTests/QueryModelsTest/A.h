#ifndef A_H
#define A_H

#include <QObject>
#include <QSharedPointer>

#include "B.h"
#include "KindA.h"

class A : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QString code_1 READ getCode WRITE setCode NOTIFY codeChanged)
  Q_PROPERTY(QSharedPointer<KindA> kindA READ getKindA WRITE setKindA NOTIFY kindAChanged)
  Q_PROPERTY(QList<QSharedPointer<B>> child READ getChild WRITE setChild)
public:
  Q_INVOKABLE explicit A(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  QString getCode() const;
  void setCode(const QString &value);

  QSharedPointer<KindA> getKindA() const;
  void setKindA(QSharedPointer<KindA> value);

  QList<QSharedPointer<B>> getChild() const;
  void setChild(const QList<QSharedPointer<B>> &value);

signals:
  void codeChanged();
  void kindAChanged();

protected:
  long id = 0;
  QString code_1;
  QSharedPointer<KindA> kindA;
  QList<QSharedPointer<B>> child;
};

#endif // A_H
