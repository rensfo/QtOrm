#include "Registry.h"

#include <QMetaObject>

#include "Mappings/ConfigurationMap.h"
#include "Mappings/SubClassMap.h"

namespace QtOrm {

Registry::Registry(QObject *parent) : QObject(parent) {
}

bool Registry::contains(const QString &table, const IdType &id) {
  if (data.contains(table)) {
    return data.value(table).contains(id);
  }

  return false;
}

void Registry::insert(const QString &table, const IdType &id, ItemType object) {
  if (data.contains(table)) {
    if (!data.value(table).contains(id)) {
      data[table].insert(id, object);
      itemsIds.insert(object, id);
    }
  } else {
    RegistryData ids;
    ids.insert(id, object);
    data.insert(table, ids);
    itemsIds.insert(object, id);
  }
}

void Registry::remove(const QString &table, const IdType &id) {
  if (contains(table, id)) {
    auto object = value(table, id);
    itemsIds.remove(object);
    data[table].remove(id);

  }
}

void Registry::remove(ItemType object) {
  for (RegistryData &ids : data) {
    for (QSharedPointer<QObject> &registryObject : ids) {
      if (registryObject == object) {
        IdType key = ids.key(object);
        ids.remove(key);
        itemsIds.remove(object);
        return;
      }
    }
  }
}

Registry::ItemType Registry::value(const QString &table, const IdType &id) {
  if (contains(table, id)) {
    return data[table][id];
  }

  return ItemType();
}

Registry::ItemType Registry::value(QObject *object) {
  QString className = object->metaObject()->className();
  QString tableName;
  auto classBase = configuration->getMappedClass(className);
  if(Mapping::SubClassMap::isClassTableInheritance(classBase)){
    classBase = classBase->toSubclass()->getBaseClass();
  }
  tableName = classBase->getTable();

  if (data.contains(tableName)) {
    for (QSharedPointer<QObject> &registryObject : data[tableName]) {
      if (registryObject.data() == object) {
        return registryObject;
      }
    }
  }

  return QSharedPointer<QObject>();
}

bool Registry::contains(Registry::ItemType object) {
  return itemsIds.contains(object);
}

Registry::IdType Registry::getId(ItemType object) {
  return itemsIds.value(object);
}

void Registry::clear() {
  data.clear();
}

QSharedPointer<Config::ConfigurationMap> Registry::getConfiguration() const
{
  return configuration;
}

void Registry::setConfiguration(QSharedPointer<Config::ConfigurationMap> value)
{
  configuration = value;
}
}

uint qHash(const QVariant&var)
{
  return qHash(var.toString());
}
