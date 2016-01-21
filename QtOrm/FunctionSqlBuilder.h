#ifndef FUNCTIONSQLBUILDER_H
#define FUNCTIONSQLBUILDER_H

#include "SqlBuilderBase.h"

namespace QtOrm {
namespace Sql {
class FunctionSqlBuilder : public SqlBuilderBase {
  Q_OBJECT

public:
  FunctionSqlBuilder(const QSqlDatabase &database, QObject *parent = 0);

  void insertObject(QObject &object);
  void updateObject(const QObject &object);
  void deleteObject(const QObject &object);

private:
  SqlBuilderBase *functionBuilder;
};
}
}
#endif // FUNCTIONSQLBUILDER_H
