#ifndef CONDITIONEQUAL_H
#define CONDITIONEQUAL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionEqual : public Condition {
public:
  ConditionEqual();
  ~ConditionEqual();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONEQUAL_H
