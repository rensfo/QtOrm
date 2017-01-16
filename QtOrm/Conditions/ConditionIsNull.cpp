#include "ConditionIsNull.h"

namespace QtOrm {
namespace Sql {

ConditionIsNull::~ConditionIsNull() {
}

QString ConditionIsNull::toSqlString(const QString &tableName, const QString &placeholder) const {
  Q_UNUSED(placeholder)

  return QString("%1.%2 is null").arg(tableName).arg(property);
}

QSharedPointer<Condition> ConditionIsNull::clone() {
  return this->cloneBase<ConditionIsNull>();
}
}
}
