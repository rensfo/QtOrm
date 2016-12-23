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
    throw NotRegistredClassException(QString("Class '%1' was not registed.").arg(className));

  return mappedClass.value(className);
}

bool ConfigurationMap::isRegisterClass(const QString &className) {
  return mappedClass.contains(className);
}

QStringList ConfigurationMap::getRegistredClasses() {
  return mappedClass.keys();
}
}
}
