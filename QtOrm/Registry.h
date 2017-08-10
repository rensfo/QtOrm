#ifndef REGISTRY_H
#define REGISTRY_H

#include <QHash>
#include <QObject>
#include <QSharedPointer>

#include "Mappings/ConfigurationMap.h"

namespace QtOrm {

class Registry : public QObject {
  Q_OBJECT

  using RegistryData = QHash<QString, QSharedPointer<QObject>>;

public:
  explicit Registry(QObject *parent = nullptr);
  bool contains(const QString &table, const QString &id);
  void insert(const QString &table, const QString &id, QSharedPointer<QObject> object);
  void remove(const QString &table, const QString &id);
  void remove(QSharedPointer<QObject> object);
  QSharedPointer<QObject> value(const QString &table, const QString &id);
  QSharedPointer<QObject> value(QObject *object);

  void clear();

  QSharedPointer<Config::ConfigurationMap> getConfiguration() const;
  void setConfiguration(QSharedPointer<Config::ConfigurationMap> value);

private:
  QHash<QString, RegistryData> data;
  QSharedPointer<Config::ConfigurationMap> configuration;
};
}
#endif // REGISTRY_H
