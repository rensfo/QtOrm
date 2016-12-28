#ifndef OPERATION_H
#define OPERATION_H

#include <QMap>

namespace QtOrm {
namespace Sql {

enum class Operation : int {
  None,
  Like,
  Equal,
  NotEqual,
  IsNull,
  IsNotNull,
  In,
  Between,
  Greater,
  GreaterOrEqual,
  Less,
  LessOrEqual
};

Q_DECLARE_FLAGS(Operations, Operation)
Q_DECLARE_OPERATORS_FOR_FLAGS(Operations)

static QMap<Operation, QString> OperationStrings = {{Operation::Like, "like"},
                                                    {Operation::Equal, "="},
                                                    {Operation::NotEqual, "!="},
                                                    {Operation::IsNull, "is null"},
                                                    {Operation::IsNotNull, "is not null"},
                                                    {Operation::In, "in"},
                                                    {Operation::Between, "between"},
                                                    {Operation::Greater, ">"},
                                                    {Operation::GreaterOrEqual, ">="},
                                                    {Operation::Less, "<"},
                                                    {Operation::LessOrEqual, "<="}};
}
}
#endif // OPERATION_H
