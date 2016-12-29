#include "ConditionLike.h"

namespace QtOrm {
namespace Sql {

ConditionLike::~ConditionLike() {
}

QString ConditionLike::toSqlString(const QString &tableName, const QString &placeholder) const {
  return QString("%1.%2 like '%' || :%3 || '%'").arg(tableName).arg(column).arg(placeholder);
}
}
}
