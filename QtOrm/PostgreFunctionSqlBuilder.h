#ifndef POSTGREFUNCTIONSQLBUILDER_H
#define POSTGREFUNCTIONSQLBUILDER_H

#include "SqlBuilderBase.h"

namespace QtOrm {
namespace Sql {

class PostgreFunctionSqlBuilder : public SqlBuilderBase {
  Q_OBJECT

public:
  PostgreFunctionSqlBuilder(const QSqlDatabase &database, QObject *parent);

  void insertObject(QObject &object);
  void updateObject(const QObject &object);
  void deleteObject(const QObject &object);

private:
  QMap<Config::PropertyMap *, QString> *getUpdateColumnText(Mapping::ClassMapBase &classBase);
  QString getUpdateFunctionText(const Mapping::PropertyMap &property, const QString &idParamName, const QString &context);
  QString getInsertFunctionText(Mapping::ClassMapBase &classBase);
  QString getDeleteFunctionText(Mapping::ClassMapBase &classBase);
  QString getFunctionText(const QString &functionName, const QString &context);
  QString wrapSelect(const QString &functionText);
  QVariant insertRecord(Mapping::ClassMapBase &classBase);
  void updateRecord(const QObject &object, Mapping::ClassMapBase &classBase);
  void deleteRecord(const QObject &object, Mapping::ClassMapBase &classBase);

private:
};
}
}
#endif // POSTGREFUNCTIONSQLBUILDER_H
