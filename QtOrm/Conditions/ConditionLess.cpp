#include "ConditionLess.h"

namespace QtOrm {
namespace Sql {

ConditionLess::ConditionLess() {
}

ConditionLess::~ConditionLess() {
}

QString ConditionLess::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 < :%3").arg(tableName).arg(column).arg(placeholder);
}
}
}
