#include "ConfigurateMap.h"

#include <QMap>
#include <QString>

namespace QtOrm {
namespace Config {

ConfigurateMap::ConfigurateMap() {
}

ClassMapBase *ConfigurateMap::getMappedClass(QString className) {
  return mappedClass.value(className);
}

bool ConfigurateMap::isRegisterClass(QString className) {
  return mappedClass.contains(className);
}

void ConfigurateMap::writeRegisteredClassesToDebug() {
  qDebug() << mappedClass;
}
}
}
