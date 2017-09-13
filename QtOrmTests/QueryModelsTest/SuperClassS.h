#ifndef SUPERCLASSS_H
#define SUPERCLASSS_H

#include <QObject>

class SuperClassS : public QObject
{
  Q_OBJECT
  Q_PROPERTY(long id READ getId WRITE setId)
  Q_PROPERTY(QString code READ getCode WRITE setCode NOTIFY codeChanged)
  Q_PROPERTY(int discriminator READ getDiscriminator WRITE setDiscriminator)

public:
  Q_INVOKABLE explicit SuperClassS(QObject *parent = nullptr);

  long getId() const;
  void setId(long value);

  QString getCode() const;
  void setCode(const QString &value);

  int getDiscriminator() const;
  void setDiscriminator(int value);

signals:
  void codeChanged();

private:
  long id = 0;
  QString code;
  int discriminator = 0;
};

#endif // SUPERCLASSS_H
