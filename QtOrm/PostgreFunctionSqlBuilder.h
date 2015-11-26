#ifndef POSTGREFUNCTIONSQLBUILDER_H
#define POSTGREFUNCTIONSQLBUILDER_H

#include "SqlBuilderBase.h"


namespace QtOrm{
namespace Sql {

class PostgreFunctionSqlBuilder : public SqlBuilderBase
{
    Q_OBJECT

public:
    PostgreFunctionSqlBuilder(const QSqlDatabase &database, QObject *parent);

    QSqlQuery insertObject(const QObject &object);
    QSqlQuery updateObject(const QObject &object);
    QSqlQuery deleteObject(const QObject &object);

private:
    QString getDeclareText(Mapping::ClassMapBase &classBase, QString paramName);
    QString getUpdateColumnText(Mapping::ClassMapBase &classBase);
    QString getAnonymousBlock(QString declare, QString body);
    QString getExecuteFunctionText(const Mapping::PropertyMap &property, const QString &idParamName, const QString &context);
    QString getInsertFunctionText(Mapping::ClassMapBase &classBase, QString paramName);
    QString getDeleteFunctionText(Mapping::ClassMapBase &classBase);
    QString getFunctionText(const QString &functionName, const QString &context);
    void bindColumns(Mapping::ClassMapBase &classBase, QSqlQuery &query, const QObject &object);

private:

};

}
}
#endif // POSTGREFUNCTIONSQLBUILDER_H
