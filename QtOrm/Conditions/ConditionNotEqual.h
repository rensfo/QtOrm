#ifndef CONDITIONNOTEQUAL_H
#define CONDITIONNOTEQUAL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionNotEqual : public Condition
{
public:
  using Condition::Condition;
  ~ConditionNotEqual();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}
#endif // CONDITIONNOTEQUAL_H
