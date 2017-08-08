#include "Registry.h"

#include <QMetaObject>
#include "ConfigurationMap.h"
#include "SubClassMap.h"

namespace QtOrm {

Registry::Registry(QObject *parent) : QObject(parent) {
}

bool Registry::contains(const QString &table, const QString &id) {
  if (data.contains(table)) {
    return data.value(table).contains(id);
  }

  return false;
}

void Registry::insert(const QString &table, const QString &id, QSharedPointer<QObject> object) {
  if (data.contains(table)) {
    if (!data.value(table).contains(id)) {
      data[table].insert(id, object);
    }
  } else {
    RegistryData ids;
    ids.insert(id, object);
    data.insert(table, ids);
  }
}

void Registry::remove(const QString &table, const QString &id) {
  if (contains(table, id)) {
    data[table].remove(id);
  }
}

void Registry::remove(QSharedPointer<QObject> object) {
  for (RegistryData &ids : data) {
    for (QSharedPointer<QObject> &registryObject : ids) {
      if (registryObject == object) {
        QString key = ids.key(object);
        ids.remove(key);
        return;
      }
    }
  }
}

QSharedPointer<QObject> Registry::value(const QString &table, const QString &id) {
  if (contains(table, id)) {
    return data[table][id];
  }

  return QSharedPointer<QObject>();
}

QSharedPointer<QObject> Registry::value(QObject *object) {
  QString className = object->metaObject()->className();
  QString tableName;
  auto classBase = Config::ConfigurationMap::getMappedClass(className);
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

void Registry::clear() {
  data.clear();
}
}
