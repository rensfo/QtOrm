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

QSqlQuery FunctionSqlBuilder::insertObject(const QObject &object) {
  return functionBuilder->insertObject(object);
}

QSqlQuery FunctionSqlBuilder::updateObject(const QObject &object) {
  return functionBuilder->updateObject(object);
}

QSqlQuery FunctionSqlBuilder::deleteObject(const QObject &object) {
  return functionBuilder->deleteObject(object);
}
}
}
