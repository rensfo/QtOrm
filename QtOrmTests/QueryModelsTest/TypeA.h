#ifndef TYPEA_H
#define TYPEA_H

#include <QObject>

class TypeA : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QString code READ getCode WRITE setCode NOTIFY codeChanged)
  Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
public:
  Q_INVOKABLE explicit TypeA(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  QString getCode() const;
  void setCode(const QString &value);

  QString getName() const;
  void setName(const QString &value);

signals:
  void codeChanged(QString);
  void nameChanged(QString);

protected:
  long id = 0;
  QString code;
  QString name;
};

#endif // TYPEA_H
