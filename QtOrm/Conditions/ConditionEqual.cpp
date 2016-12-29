#include "ConditionEqual.h"

namespace QtOrm {
namespace Sql {

ConditionEqual::ConditionEqual() : Condition() {
}

ConditionEqual::~ConditionEqual() {
}

QString ConditionEqual::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 = :%3").arg(tableName).arg(column).arg(placeholder);
}
}
}
