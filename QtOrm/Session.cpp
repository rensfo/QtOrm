#include "FunctionSqlBuilder.h"
#include "Session.h"

namespace QtOrm {
Session::Session(const QSqlDatabase &database,
                 Sql::SqlBuilderType sqlManagerType,
                 QObject *parent)
    : QObject(parent), database(database), sqlBuilder(nullptr), textStream(nullptr) {
  //Должна быть фабрика, если потребуется
  switch (sqlManagerType) {
  case Sql::SqlBuilderType::Simple:
    sqlBuilder = new Sql::SimpleSqlBuilder(database);
    break;
  case Sql::SqlBuilderType::Setter:
      sqlBuilder = new Sql::FunctionSqlBuilder(database);
    break;
  }
}

void Session::insertObject(QObject &object) {
  sqlBuilder->insertObject(object);
}

void Session::updateObject(const QObject &object) {
  sqlBuilder->updateObject(object);
}

void Session::deleteObject(const QObject &object) {
    sqlBuilder->deleteObject(object);
}

QSqlDatabase Session::getDatabase() const {
  return database;
}

void Session::setDatabase(const QSqlDatabase &database) {
    this->database = database;
}

QTextStream *Session::getTextStream() const
{
    return sqlBuilder->getTextStream();
}

void Session::setTextStream(QTextStream *value)
{
    sqlBuilder->setTextStream(value);
}
}
