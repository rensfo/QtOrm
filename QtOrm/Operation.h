#ifndef OPERATION_H
#define OPERATION_H

#include <QMap>

enum class Operation : int { None = 0, Like = 1, Equal = 2, NotEqual = 4, IsNull = 16, IsNotNull = 32 };

Q_DECLARE_FLAGS(Operations, Operation)
Q_DECLARE_OPERATORS_FOR_FLAGS(Operations)

static QMap<Operation, QString> OperationSimbols = {
    {Operation::Like, "like"},
    {Operation::Equal, "="},
    {Operation::NotEqual, "!="},
    {Operation::IsNull, "is null"},
    {Operation::IsNotNull, "is null"}
};

#endif // OPERATION_H
