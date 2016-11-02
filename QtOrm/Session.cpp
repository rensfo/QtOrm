#include "Session.h"
#include "SqlBuilderBase.h"

#include <QDebug>

namespace QtOrm {
Session::Session(QObject *parent) : QObject(parent) {
  auto simpleSqlBuilder = new SimpleSqlBuilder();
  query.setSqlBuilder(simpleSqlBuilder);
  connect(&query, SIGNAL(executeSql(QString)), SIGNAL(executeSql(QString)));
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

bool Session::getAutoUpdate() const {
  return query.getAutoUpdate();
}

void Session::setAutoUpdate(bool value) {
  query.setAutoUpdate(value);
}
}
