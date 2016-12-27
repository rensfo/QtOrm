#include "Session.h"
#include "SqlBuilderBase.h"

#include <QDebug>

namespace QtOrm {
Session::Session(QObject *parent) : QObject(parent) {
  reestr = QSharedPointer<Reestr>::create();
  queryCache = QSharedPointer<QueryCache>::create();

  updater = QSharedPointer<AutoUpdater>::create();
  updater->setReestr(reestr);
  updater->setQueryCache(queryCache);
  connect(updater.data(), &AutoUpdater::executedSql, this, &Session::executedSql);
}

QSqlDatabase Session::getDatabase() const {
  return database;
}

void Session::setDatabase(const QSqlDatabase &database) {
  this->database = database;
  updater->setDatabase(this->database);
}

bool Session::getAutoUpdate() const {
  return autoUpdate;
}

void Session::setAutoUpdate(bool value) {
  if (autoUpdate != value) {
    autoUpdate = value;
  }
}

void Session::clearReestr() {
  reestr->clear();
}

void Session::clearQueryCache()
{
  queryCache->clear();
}

void Session::removeFromCache(QSharedPointer<QObject> object)
{
  if(reestr) {
    reestr->remove(object);
  }
}

Query Session::createQuery() {
  Query query;
  query.setDatabase(database);
  query.setReestr(reestr);
  query.setQueryCache(queryCache);
  if(autoUpdate){
    query.setUpdater(updater);
  }

  connect(&query, &Query::executedSql, this, &Session::executedSql);

  return query;
}
}
