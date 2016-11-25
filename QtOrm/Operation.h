#ifndef OPERATION_H
#define OPERATION_H

#include <QMap>

namespace QtOrm {
namespace Sql {

enum class Operation : int { None = 0, Like = 1, Equal = 2, NotEqual = 4, IsNull = 16, IsNotNull = 32, In = 64, Between = 128 };

Q_DECLARE_FLAGS(Operations, Operation)
Q_DECLARE_OPERATORS_FOR_FLAGS(Operations)

static QMap<Operation, QString> OperationStrings = {
    {Operation::Like, "like"},
    {Operation::Equal, "="},
    {Operation::NotEqual, "!="},
    {Operation::IsNull, "is null"},
    {Operation::IsNotNull, "is not null"},
    {Operation::In, "in"},
    {Operation::Between, "between"}
};

}
}
#endif // OPERATION_H
