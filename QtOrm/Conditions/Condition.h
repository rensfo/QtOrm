#ifndef CONDITION_H
#define CONDITION_H

#include <QVariant>

#include "Operation.h"

namespace QtOrm {
namespace Sql {

class Condition {
public:
  Condition();
  Condition(const QString &property, const QVariant &value);
  Condition(const Condition &other);
  virtual ~Condition();

  QVariantList getValues() const;
  void setValues(const QVariantList &value);
  void setValue(const QVariant &value);
  void clearValues();

  QString getColumn() const;
  void setColumn(const QString &value);

  QString getOperationSymbol() const;

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const = 0;

  QString getProperty() const;
  void setProperty(const QString &value);

protected:
  QString property;
  QString column;
  QVariantList values;
  QString operationSymbol;
};
}
}
#endif // CONDITION_H
