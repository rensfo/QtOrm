#ifndef CONDITION_H
#define CONDITION_H

#include <QSharedPointer>
#include <QVariant>

#include "Operation.h"

namespace QtOrm {
namespace Sql {

class Condition {
public:
  Condition();
  Condition(const QString &property, Operation operation, const QVariant &value);
  Condition(const QString &property, Operation operation, const QVariantList &values);
  Condition(const Condition &other);
  virtual ~Condition();

  QVariantList getValues() const;
  void setValues(const QVariantList &value);
  void setValue(const QVariant &value);
  void clearValues();

  QString getProperty() const;
  void setProperty(const QString &value);

  Operation getOperation() const;
  void setOperation(const Operation &value);

protected:
  QString property;
  Operation operation = Operation::Equal;
  QVariantList values;
};
}
}
#endif // CONDITION_H
