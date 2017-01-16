#include "ConditionNotEqual.h"

namespace QtOrm {
namespace Sql {

ConditionNotEqual::~ConditionNotEqual() {
}

QString ConditionNotEqual::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 != :%3").arg(tableName).arg(property).arg(placeholder);
}

QSharedPointer<Condition> ConditionNotEqual::clone() {
  return this->cloneBase<ConditionNotEqual>();
}
}
}
