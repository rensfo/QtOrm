#ifndef CONDITIONFACTORY_H
#define CONDITIONFACTORY_H

#include <QSharedPointer>

#include "Condition.h"

namespace QtOrm {
namespace Sql {

class ConditionFactory {
public:
  static QSharedPointer<Condition> create(const QString &property, Operation operation, const QVariant &value);
  static QSharedPointer<Condition> create(const QString &property, Operation operation, const QVariantList &values);
};
}
}

#endif // CONDITIONFACTORY_H
