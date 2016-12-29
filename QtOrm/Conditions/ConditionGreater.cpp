#include "ConditionGreater.h"

namespace QtOrm {
namespace Sql {

ConditionGreater::ConditionGreater() {
}

ConditionGreater::~ConditionGreater() {
}

QString ConditionGreater::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 > :%3").arg(tableName).arg(column).arg(placeholder);
}
}
}
