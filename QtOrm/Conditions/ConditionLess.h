#ifndef CONDITIONLESS_H
#define CONDITIONLESS_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionLess : public Condition {
public:
  using Condition::Condition;
  ~ConditionLess();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}

#endif // CONDITIONLESS_H
