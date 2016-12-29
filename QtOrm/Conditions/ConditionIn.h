#ifndef CONDITIONIN_H
#define CONDITIONIN_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionIn : public Condition
{
public:
  ConditionIn();
  ~ConditionIn();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONIN_H
