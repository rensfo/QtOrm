#ifndef CONDITIONGREATEROREQUAL_H
#define CONDITIONGREATEROREQUAL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionGreaterOrEqual : public Condition {
public:
  using Condition::Condition;
  ~ConditionGreaterOrEqual();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}

#endif // CONDITIONGREATEROREQUAL_H
