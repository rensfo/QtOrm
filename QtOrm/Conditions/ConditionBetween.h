#ifndef CONDITIONBETWEEN_H
#define CONDITIONBETWEEN_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionBetween : public Condition {
public:
  using Condition::Condition;
  ~ConditionBetween();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}
#endif // CONDITIONBETWEEN_H
