#include "FunctionSqlBuilder.h"
#include "PostgreFunctionSqlBuilder.h"

namespace QtOrm {
namespace Sql {

FunctionSqlBuilder::FunctionSqlBuilder(const QSqlDatabase &database, QObject *parent)
    : SqlBuilderBase(database, parent) {
  if (database.driverName() == "QPSQL")
    functionBuilder = new PostgreFunctionSqlBuilder(database, this);
  else
    throw "Function Sql Buider: not supported driver " + database.driverName();
}

void FunctionSqlBuilder::insertObject(QObject &object) {
  functionBuilder->insertObject(object);
}

void FunctionSqlBuilder::updateObject(const QObject &object) {
  functionBuilder->updateObject(object);
}

void FunctionSqlBuilder::deleteObject(const QObject &object) {
  functionBuilder->deleteObject(object);
}
}
}
