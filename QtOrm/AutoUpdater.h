#ifndef AUTOUPDATER_H
#define AUTOUPDATER_H

#include <QMetaProperty>
#include <QObject>
#include <QSqlDatabase>

#include "ConfigurationMap.h"
#include "Registry.h"

namespace QtOrm {

class AutoUpdater : public QObject {
  Q_OBJECT
public:
  explicit AutoUpdater(QObject *parent = nullptr);

  QSharedPointer<Registry> getRegistry() const;
  void setRegistry(QSharedPointer<Registry> value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  QSharedPointer<Config::ConfigurationMap> getConfiguration() const;
  void setConfiguration(QSharedPointer<Config::ConfigurationMap> value);

protected:
  QString getPropertyName(QSharedPointer<QObject> sender, int senderSignalIndex);

signals:
  void executedSql(QString);

public slots:
  void connectToAllProperties(QSharedPointer<QObject> object);

protected slots:
  void onObjectPropertyChanged();

protected:
  QMetaMethod findOnObjectPropertyChangedMethod();

protected:
  QMetaMethod onObjectPropertyChangedMethod;
  QSharedPointer<Registry> registry;
  QSqlDatabase database;
  QSharedPointer<Config::ConfigurationMap> configuration;
};
}

#endif // AUTOUPDATER_H
