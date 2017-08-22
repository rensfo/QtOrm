#ifndef REGISTRY_H
#define REGISTRY_H



#include <QHash>
#include <QObject>
#include <QSharedPointer>

#include "Mappings/ConfigurationMap.h"

uint qHash( const QVariant & var );

namespace QtOrm {

class Registry : public QObject {
  Q_OBJECT

public:
  using IdType = QVariant;
  using ItemType = QSharedPointer<QObject>;
private:
  using RegistryData = QHash<IdType, ItemType>;

public:
  explicit Registry(QObject *parent = nullptr);
  bool contains(const QString &table, const IdType &id);
  void insert(const QString &table, const IdType &id, ItemType object);
  void remove(const QString &table, const IdType &id);
  void remove(ItemType object);
  ItemType value(const QString &table, const IdType &id);
  ItemType value(QObject *object);

  bool contains(ItemType object);
  IdType getId(ItemType object);

  void clear();

  QSharedPointer<Config::ConfigurationMap> getConfiguration() const;
  void setConfiguration(QSharedPointer<Config::ConfigurationMap> value);

private:
  QHash<IdType, RegistryData> data;
  QSharedPointer<Config::ConfigurationMap> configuration;
  QHash<ItemType, IdType> itemsIds;
};
}
#endif // REGISTRY_H
