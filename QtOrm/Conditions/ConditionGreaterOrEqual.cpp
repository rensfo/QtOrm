#include "ConditionGreaterOrEqual.h"

namespace QtOrm {
namespace Sql {

ConditionGreaterOrEqual::~ConditionGreaterOrEqual() {
}

QString ConditionGreaterOrEqual::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 >= :%3").arg(tableName).arg(property).arg(placeholder);
}

QSharedPointer<Condition> ConditionGreaterOrEqual::clone() {
  return this->cloneBase<ConditionGreaterOrEqual>();
}
}
}
