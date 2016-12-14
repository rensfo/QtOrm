#ifndef KINDA_H
#define KINDA_H

#include "TypeA.h"

#include <QObject>
#include <QSharedPointer>

class KindA : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QString code READ getCode WRITE setCode NOTIFY codeChanged)
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QSharedPointer<TypeA> typeA READ getTypeA WRITE setTypeA NOTIFY typeAChanged)
public:
  Q_INVOKABLE explicit KindA(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  QString getCode() const;
  void setCode(const QString &value);

  QString getName() const;
  void setName(const QString &value);

  QSharedPointer<TypeA> getTypeA() const;
  void setTypeA(QSharedPointer<TypeA> value);

signals:
  void codeChanged();
  void nameChanged();
  void typeAChanged();

protected:
  long id = 0;
  QString code;
  QString name;
  QSharedPointer<TypeA> typeA;
};

#endif // KINDA_H
