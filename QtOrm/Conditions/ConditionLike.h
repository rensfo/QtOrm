#ifndef CONDITIONLIKE_H
#define CONDITIONLIKE_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionLike : public Condition {
public:
  using Condition::Condition;
  ~ConditionLike();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONLIKE_H
