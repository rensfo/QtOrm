#include "ConditionLess.h"

namespace QtOrm {
namespace Sql {

ConditionLess::~ConditionLess() {
}

QString ConditionLess::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 < :%3").arg(tableName).arg(property).arg(placeholder);
}

QSharedPointer<Condition> ConditionLess::clone() {
  return this->cloneBase<ConditionLess>();
}
}
}
