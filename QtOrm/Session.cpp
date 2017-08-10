#include "Session.h"
#include "SqlBuilderBase.h"

#include <QDebug>

namespace QtOrm {
Session::Session(QObject *parent) : QObject(parent) {
  configuration = QSharedPointer<Config::ConfigurationMap>::create();
  registry = QSharedPointer<Registry>::create();
  registry->setConfiguration(configuration);

  updater = QSharedPointer<AutoUpdater>::create();
  updater->setRegistry(registry);
  updater->setConfiguration(configuration);
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

void Session::clearRegistry() {
  registry->clear();
}

void Session::removeFromCache(QSharedPointer<QObject> object) {
  if (registry) {
    registry->remove(object);
  }
}

Query Session::createQuery() {
  Query query;
  query.setDatabase(database);
  query.setRegistry(registry);
  query.setConfiguration(configuration);
  if (autoUpdate) {
    query.setUpdater(updater);
  }

  connect(&query, &Query::executedSql, this, &Session::executedSql);

  return query;
}

QSharedPointer<Config::ConfigurationMap> Session::getConfiguration() const {
    return configuration;
}
}
