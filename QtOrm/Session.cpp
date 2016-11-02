#include "Session.h"
#include "FunctionSqlBuilder.h"
#include "SqlBuilderBase.h"

#include <QDebug>

namespace QtOrm {
Session::Session(QObject *parent) : QObject(parent) {
}

void Session::saveObject(QObject &object) {
  query.saveObject(object);
}

void Session::deleteObject(QObject &object) {
  query.deleteObject(object);
}

void Session::refresh(QObject &value) {
  query.refresh(value);
}

QSqlDatabase Session::getDatabase() const {
  return database;
}

void Session::setDatabase(const QSqlDatabase &database) {
  this->database = database;
  query.setDatabase(this->database);
}
Sql::SqlBuilderType Session::getSqlBuilderType() const {
  return sqlBuilderType;
}

void Session::setSqlBuilderType(const Sql::SqlBuilderType &value) {
  sqlBuilderType = value;
  //Должна быть фабрика, если потребуется
  switch (value) {
  case SqlBuilderType::Simple: {
    auto simpleSqlBuilder = new SimpleSqlBuilder();
    simpleSqlBuilder->setDatabase(database);
    query.setSqlBuilder(simpleSqlBuilder);
    break;
  }
  case Sql::SqlBuilderType::Setter: {
    auto setterSqlBuilder = new FunctionSqlBuilder();
    setterSqlBuilder->setDatabase(database);
    query.setSqlBuilder(setterSqlBuilder);
    break;
  }
  }
  connect(&query, SIGNAL(executeSql(QString)), SIGNAL(executeSql(QString)));
}

bool Session::getAutoUpdate() const {
  return query.getAutoUpdate();
}

void Session::setAutoUpdate(bool value) {
  query.setAutoUpdate(value);
}
}
