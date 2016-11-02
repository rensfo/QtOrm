#ifndef FUNCTIONSQLBUILDER_H
#define FUNCTIONSQLBUILDER_H

#include "SqlBuilderBase.h"

namespace QtOrm {
namespace Sql {
class FunctionSqlBuilder : public SqlBuilderBase {
  Q_OBJECT

public:
  FunctionSqlBuilder(QObject *parent = nullptr);

  virtual QSqlQuery insertQuery() override;
  virtual QSqlQuery updateQuery() override;
  virtual QSqlQuery deleteQuery() override;
  virtual QSqlQuery updateOneColumnQuery(const QString &property) override;

private:
  SqlBuilderBase *functionBuilder;
};
}
}
#endif // FUNCTIONSQLBUILDER_H
