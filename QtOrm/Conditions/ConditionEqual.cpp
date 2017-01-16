#include "ConditionEqual.h"

namespace QtOrm {
namespace Sql {

ConditionEqual::~ConditionEqual() {
}

QString ConditionEqual::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 = :%3").arg(tableName).arg(property).arg(placeholder);
}

QSharedPointer<Condition> ConditionEqual::clone() {
  return this->cloneBase<ConditionEqual>();
}
}
}
