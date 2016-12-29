#include "ConditionNotEqual.h"

namespace QtOrm {
namespace Sql {

ConditionNotEqual::ConditionNotEqual() : Condition() {
}

ConditionNotEqual::~ConditionNotEqual() {
}

QString ConditionNotEqual::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 != :%3").arg(tableName).arg(column).arg(placeholder);
}
}
}
