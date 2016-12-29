#ifndef CONDITIONLESSOREQUAL_H
#define CONDITIONLESSOREQUAL_H

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionLessOrEqual : public Condition {
public:
  ConditionLessOrEqual();
  ~ConditionLessOrEqual();

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const override;
};
}
}

#endif // CONDITIONLESSOREQUAL_H
