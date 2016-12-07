#include "Session.h"
#include "SqlBuilderBase.h"

#include <QDebug>

namespace QtOrm {
Session::Session(QObject *parent) : QObject(parent) {
  reestr = new Reestr(this);

  queryCache = new QueryCache(this);

  updater.setReestr(reestr);
  connect(&updater, &AutoUpdater::executedSql, this, &Session::executedSql);
}

void Session::saveObject(QObject &object) {
  Query query = createQuery();
  query.saveObject(object);
}

void Session::deleteObject(QObject &object) {
  Query query = createQuery();
  query.deleteObject(object);
}

void Session::refresh(QObject &value) {
  Query query = createQuery();
  query.refresh(value);
}

QSqlDatabase Session::getDatabase() const {
  return database;
}

void Session::setDatabase(const QSqlDatabase &database) {
  this->database = database;
  updater.setDatabase(this->database);
}

bool Session::getAutoUpdate() const {
  return autoUpdate;
}

void Session::setAutoUpdate(bool value) {
  if (autoUpdate != value) {
    autoUpdate = value;
    if (autoUpdate) {
      connect(reestr, &Reestr::inserted, &updater, &AutoUpdater::connectToAllProperties);
    } else {
      disconnect(reestr, &Reestr::inserted, &updater, &AutoUpdater::connectToAllProperties);
    }
  }
}

void Session::clearReestr() {
  reestr->clear();
}

void Session::clearQueryCache()
{
  queryCache->clear();
}

void Session::removeFromCache(QObject *object)
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

  connect(&query, &Query::executedSql, this, &Session::executedSql);

  return query;
}
}
