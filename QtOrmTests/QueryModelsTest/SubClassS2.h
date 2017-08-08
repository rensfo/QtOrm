#ifndef SUBCLASSS2_H
#define SUBCLASSS2_H

#include "SuperClassS.h"

class SubClassS2 : public SuperClassS
{
  Q_OBJECT
  Q_PROPERTY(QString strVal READ getStrVal WRITE setStrVal NOTIFY strValChanged)

public:
  Q_INVOKABLE explicit SubClassS2(QObject *parent = nullptr);

  QString getStrVal() const;
  void setStrVal(const QString &value);

signals:
  void strValChanged();

private:
  QString strVal;
};

#endif // SUBCLASSS2_H
