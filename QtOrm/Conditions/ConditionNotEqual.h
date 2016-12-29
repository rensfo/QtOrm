#ifndef CONDITIONNOTEQUAL_H
#define CONDITIONNOTEQUAL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionNotEqual : public Condition
{
public:
  ConditionNotEqual();
  ~ConditionNotEqual();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}
#endif // CONDITIONNOTEQUAL_H
