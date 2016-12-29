#ifndef CONDITIONBETWEEN_H
#define CONDITIONBETWEEN_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionBetween : public Condition {
public:
  ConditionBetween();
  ~ConditionBetween();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}
#endif // CONDITIONBETWEEN_H
