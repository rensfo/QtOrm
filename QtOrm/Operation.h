#ifndef OPERATION_H
#define OPERATION_H

#include <QMap>

#if QT_VERSION >= 0x050000

enum class Operation { None = 0x0, Like = 0x1, Equal = 0x2, NotEqual = 0x4 };

#else

enum Operation { None = 0x0, Like = 0x1, Equal = 0x2, NotEqual = 0x4 };

#endif

Q_DECLARE_FLAGS(Operations, Operation)
Q_DECLARE_OPERATORS_FOR_FLAGS(Operations)

#if QT_VERSION >= 0x050000
static QMap<Operation, QString> OperationSimbols = {
    {Operation::Like, "like"},
    {Operation::Equal, "="},
    {Operation::NotEqual, "!="}};
#else
static QMap<Operation, QString> OperationSimbols =
    QMap<Operation, QString>({{Operation::Like, "like"},
                              {Operation::Equal, "="},
                              {Operation::NotEqual, "!="}});
#endif

#endif // OPERATION_H
