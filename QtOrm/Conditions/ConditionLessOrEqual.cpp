#include "ConditionLessOrEqual.h"

namespace QtOrm {
namespace Sql {

ConditionLessOrEqual::~ConditionLessOrEqual() {
}

QString ConditionLessOrEqual::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 <= :%3").arg(tableName).arg(property).arg(placeholder);
}

QSharedPointer<Condition> ConditionLessOrEqual::clone() {
  return this->cloneBase<ConditionLessOrEqual>();
}
}
}
