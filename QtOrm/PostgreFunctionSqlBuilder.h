#ifndef POSTGREFUNCTIONSQLBUILDER_H
#define POSTGREFUNCTIONSQLBUILDER_H

#include "SqlBuilderBase.h"

namespace QtOrm {
namespace Sql {

class PostgreFunctionSqlBuilder : public SqlBuilderBase {
  Q_OBJECT
public:
  explicit PostgreFunctionSqlBuilder(QObject *parent = nullptr);
  virtual QSqlQuery insertQuery() override;
  virtual QSqlQuery updateQuery() override;
  virtual QSqlQuery deleteQuery() override;
  virtual QSqlQuery updateOneColumnQuery(const QString &property) override;

  void deleteObject(QObject &object);

protected:
  void insertObject(QObject &object);
  void updateObject(QObject &object);

private:
  QMap<Config::PropertyMap *, QString> *
  getUpdateColumnText(Mapping::ClassMapBase &classBase);
  QString getUpdateFunctionText(const Mapping::PropertyMap &property,
                                const QString &idParamName,
                                const QString &context);
  QString getInsertFunctionText(Mapping::ClassMapBase &classBase);
  QString getDeleteFunctionText(Mapping::ClassMapBase &classBase);
  QString getFunctionText(const QString &functionName, const QString &context);
  QString wrapSelect(const QString &functionText);
  QVariant insertRecord(Mapping::ClassMapBase &classBase);
  void updateRecord(const QObject &object, Mapping::ClassMapBase &classBase);
  void deleteRecord(const QObject &object, Mapping::ClassMapBase &classBase);
};
}
}
#endif // POSTGREFUNCTIONSQLBUILDER_H
