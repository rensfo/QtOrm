#ifndef B_H
#define B_H

#include <QObject>
#include <QSharedPointer>

class A;

class B : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QSharedPointer<A> a READ getA WRITE setA NOTIFY aChanged)
  Q_PROPERTY(QString code READ getCode WRITE setCode NOTIFY codeChanged)
public:
  Q_INVOKABLE explicit B(QObject *parent = nullptr);
  ~B();

  long getId() const;
  void setId(long value);

  QString getCode() const;
  void setCode(const QString &value);

  QSharedPointer<A> getA() const;
  void setA(const QSharedPointer<A> &value);

signals:
  void aChanged();
  void codeChanged();

protected:
  long id = 0;
  QSharedPointer<A> a;
  QString code;
};

#endif // B_H
