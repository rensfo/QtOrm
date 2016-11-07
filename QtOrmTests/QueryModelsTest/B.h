#ifndef B_H
#define B_H

#include <QObject>

class B : public QObject {
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(long idA READ getIdA WRITE setIdA NOTIFY idAChanged)
  Q_PROPERTY(QString code READ getCode WRITE setCode NOTIFY codeChanged)
public:
  Q_INVOKABLE explicit B(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  long getIdA() const;
  void setIdA(long value);

  QString getCode() const;
  void setCode(const QString &value);
signals:
  void idAChanged();
  void codeChanged();

protected:
  long id = 0;
  long idA = 0;
  QString code;
};

#endif // B_H
