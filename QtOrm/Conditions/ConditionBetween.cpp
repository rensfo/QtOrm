#include "ConditionBetween.h"

namespace QtOrm {
namespace Sql {

ConditionBetween::~ConditionBetween() {
}

QString ConditionBetween::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 between :%3_1 and :%3_2 ").arg(tableName).arg(property).arg(placeholder);
}

QSharedPointer<Condition> ConditionBetween::clone() {
  return this->cloneBase<ConditionBetween>();
}
}
}
