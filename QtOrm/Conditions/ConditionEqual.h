#ifndef CONDITIONEQUAL_H
#define CONDITIONEQUAL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionEqual : public Condition {
public:
  using Condition::Condition;
  ~ConditionEqual();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}

#endif // CONDITIONEQUAL_H
