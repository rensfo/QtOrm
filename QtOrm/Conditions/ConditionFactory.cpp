#include "ConditionBetween.h"
#include "ConditionEqual.h"
#include "ConditionFactory.h"
#include "ConditionGreater.h"
#include "ConditionGreaterOrEqual.h"
#include "ConditionIn.h"
#include "ConditionIsNotNull.h"
#include "ConditionIsNull.h"
#include "ConditionLess.h"
#include "ConditionLessOrEqual.h"
#include "ConditionLike.h"
#include "ConditionNotEqual.h"

namespace QtOrm {
namespace Sql {

QSharedPointer<Condition> ConditionFactory::create(const QString &property, Operation operation,
                                                   const QVariant &value) {
  return create(property, operation, QVariantList{value});
}

QSharedPointer<Condition> ConditionFactory::create(const QString &property, Operation operation,
                                                   const QVariantList &values) {

  Operation rightOperation = operation;

  bool conditionIsNull = values.first().isNull();
  if (operation == Operation::Equal && conditionIsNull) {
    rightOperation = Operation::IsNull;
  }

  if (operation == Operation::NotEqual && conditionIsNull) {
    rightOperation = Operation::IsNotNull;
  }

  QSharedPointer<Condition> condition;

  switch (rightOperation) {
  case Operation::Like:
    condition = QSharedPointer<ConditionLike>::create();
    break;
  case Operation::Equal:
    condition = QSharedPointer<ConditionEqual>::create();
    break;
  case Operation::NotEqual:
    condition = QSharedPointer<ConditionNotEqual>::create();
    break;
  case Operation::IsNull:
    condition = QSharedPointer<ConditionIsNull>::create();
    break;
  case Operation::IsNotNull:
    condition = QSharedPointer<ConditionIsNotNull>::create();
    break;
  case Operation::In:
    condition = QSharedPointer<ConditionIn>::create();
    break;
  case Operation::Between:
    condition = QSharedPointer<ConditionBetween>::create();
    break;
  case Operation::Greater:
    condition = QSharedPointer<ConditionGreater>::create();
    break;
  case Operation::GreaterOrEqual:
    condition = QSharedPointer<ConditionGreaterOrEqual>::create();
    break;
  case Operation::Less:
    condition = QSharedPointer<ConditionLess>::create();
    break;
  case Operation::LessOrEqual:
    condition = QSharedPointer<ConditionLessOrEqual>::create();
    break;
  }

  //  condition->setColumn(column);
  condition->setProperty(property);
  condition->setValues(values);

  return condition;
}
}
}
