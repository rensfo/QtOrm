#ifndef CONDITIONISNOTNULL_H
#define CONDITIONISNOTNULL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionIsNotNull : public Condition {
public:
  ConditionIsNotNull();
  ~ConditionIsNotNull();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONISNOTNULL_H
