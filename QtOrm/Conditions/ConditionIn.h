#ifndef CONDITIONIN_H
#define CONDITIONIN_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionIn : public Condition
{
public:
  using Condition::Condition;
  ~ConditionIn();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}

#endif // CONDITIONIN_H
