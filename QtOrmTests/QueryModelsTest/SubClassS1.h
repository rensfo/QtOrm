#ifndef SUBCLASSS1_H
#define SUBCLASSS1_H

#include "SuperClassS.h"

class SubClassS1 : public SuperClassS
{
  Q_OBJECT
  Q_PROPERTY(int intVal READ getIntVal WRITE setIntVal NOTIFY intValChanged)

public:
  Q_INVOKABLE explicit  SubClassS1(QObject *parent = nullptr);

  int getIntVal() const;
  void setIntVal(int value);

signals:
  void intValChanged();

private:
  int intVal;
};

#endif // SUBCLASSS1_H
