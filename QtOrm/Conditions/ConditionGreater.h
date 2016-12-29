#ifndef CONDITIONGREATER_H
#define CONDITIONGREATER_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionGreater : public Condition {
public:
  ConditionGreater();
  ~ConditionGreater();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONGREATER_H
