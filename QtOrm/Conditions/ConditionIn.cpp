#include "ConditionIn.h"

namespace QtOrm {
namespace Sql {

ConditionIn::~ConditionIn() {
}

QString QtOrm::Sql::ConditionIn::toSqlString(const QString &tableName, const QString &placeholder) const {
  QStringList placeholders;
  for (int i = 1; i <= values.count(); ++i) {
    placeholders << QString(":%1_%2").arg(placeholder).arg(i);
  }

  return QString("%1.%2  in (%3)").arg(tableName).arg(property).arg(placeholders.join(", "));
}

QSharedPointer<QtOrm::Sql::Condition> QtOrm::Sql::ConditionIn::clone() {
  return this->cloneBase<ConditionIn>();
}
}
}