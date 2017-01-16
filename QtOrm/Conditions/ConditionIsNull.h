#ifndef CONDITIONISNULL_H
#define CONDITIONISNULL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionIsNull : public Condition {
public:
  using Condition::Condition;
  ~ConditionIsNull();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}

#endif // CONDITIONISNULL_H
