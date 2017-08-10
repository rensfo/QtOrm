#include "ConfigurationMap.h"

#include <QMap>
#include <QString>
#include <QDebug>

#include "SubClassMap.h"

namespace QtOrm {
namespace Config {

QMap<QString, QSharedPointer<ClassMapBase>> ConfigurationMap::mappedClass;
QMultiMap<QString, QSharedPointer<ClassMapBase>> ConfigurationMap::derrivedClasses;

ConfigurationMap::ConfigurationMap() {
}

void ConfigurationMap::removeAllMappings() {
  mappedClass.clear();
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

}
}
