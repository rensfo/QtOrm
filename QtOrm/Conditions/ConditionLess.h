#ifndef CONDITIONLESS_H
#define CONDITIONLESS_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionLess : public Condition {
public:
  ConditionLess();
  ~ConditionLess();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONLESS_H
