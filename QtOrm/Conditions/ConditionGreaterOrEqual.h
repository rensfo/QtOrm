#ifndef CONDITIONGREATEROREQUAL_H
#define CONDITIONGREATEROREQUAL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionGreaterOrEqual : public Condition {
public:
  ConditionGreaterOrEqual();
  ~ConditionGreaterOrEqual();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONGREATEROREQUAL_H
