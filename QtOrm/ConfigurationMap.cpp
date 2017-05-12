#include "ConfigurationMap.h"

#include <QMap>
#include <QString>
#include <QDebug>

namespace QtOrm {
namespace Config {

QMap<QString, QSharedPointer<ClassMapBase>> ConfigurationMap::mappedClass;

ConfigurationMap::ConfigurationMap() {
}

QSharedPointer<ClassMapBase> ConfigurationMap::getMappedClass(const QString &className) {
  if (!isRegisterClass(className))
    throw NotRegistredClassException(QString("Class '%1' did not registed.").arg(className));

  return mappedClass.value(className);
}

QSharedPointer<ClassMapBase> ConfigurationMap::getMappedClass(const QSharedPointer<QObject> &object) {
  return getMappedClass(object->metaObject()->className());
}

bool ConfigurationMap::isRegisterClass(const QString &className) {
  return mappedClass.contains(className);
}

QStringList ConfigurationMap::getRegistredClasses() {
  return mappedClass.keys();
}

QList<QSharedPointer<ClassMapBase>> ConfigurationMap::getDerrivedClasses(const QString& value)
{
  QList<QSharedPointer<ClassMapBase>> derrivedClasses;
  for(auto map : mappedClass.values())
  {
    if(map->isSubclass() && map->getSuperClassName() == value)
      derrivedClasses << map;
  }

  return derrivedClasses;
}

bool ConfigurationMap::isBaseClass(const QString&value)
{
  return !getDerrivedClasses(value).isEmpty();
}
}
}
