#include "ConditionIsNotNull.h"

namespace QtOrm {
namespace Sql {

QtOrm::Sql::ConditionIsNotNull::~ConditionIsNotNull() {
}

QString ConditionIsNotNull::toSqlString(const QString &tableName, const QString &placeholder) const {
  Q_UNUSED(placeholder)

  return QString("%1.%2 is not null").arg(tableName).arg(property);
}

QSharedPointer<Condition> ConditionIsNotNull::clone() {
  return this->cloneBase<ConditionIsNotNull>();
}
}
}
