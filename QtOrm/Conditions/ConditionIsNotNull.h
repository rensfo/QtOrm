#ifndef CONDITIONISNOTNULL_H
#define CONDITIONISNOTNULL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionIsNotNull : public Condition {
public:
  using Condition::Condition;
  ~ConditionIsNotNull();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
  virtual QSharedPointer<Condition> clone() override;
};
}
}

#endif // CONDITIONISNOTNULL_H
