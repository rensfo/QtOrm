#ifndef OPERATION_H
#define OPERATION_H

#include <QMap>

enum class Operation { None = 0x0, Like = 0x1, Equal = 0x2, NotEqual = 0x4 };

Q_DECLARE_FLAGS(Operations, Operation)
Q_DECLARE_OPERATORS_FOR_FLAGS(Operations)

static QMap<Operation, QString> OperationSimbols = {
    {Operation::Like, "like"}, {Operation::Equal, "="}, {Operation::NotEqual, "!="}};

#endif // OPERATION_H
