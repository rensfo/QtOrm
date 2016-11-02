#include "FunctionSqlBuilder.h"
#include "PostgreFunctionSqlBuilder.h"

namespace QtOrm {
namespace Sql {

FunctionSqlBuilder::FunctionSqlBuilder(QObject *parent)
    : SqlBuilderBase(parent) {
  if (database.driverName() == "QPSQL") {
    functionBuilder = new PostgreFunctionSqlBuilder(this);
    functionBuilder->setDatabase(database);
  } else {
    throw Exception(ErrorGroup::Sql,
                    "Function Sql Buider: not supported driver " +
                        database.driverName());
  }
}

QSqlQuery FunctionSqlBuilder::insertQuery() {
  //  functionBuilder->saveObject(object);
}

QSqlQuery FunctionSqlBuilder::updateQuery() {
  //  functionBuilder->saveObject(object);
}

QSqlQuery FunctionSqlBuilder::deleteQuery() {
  //  functionBuilder->deleteObject(object);
}

QSqlQuery FunctionSqlBuilder::updateOneColumnQuery(const QString &property) {}
}
}
