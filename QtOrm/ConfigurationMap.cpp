#include "ConfigurationMap.h"

#include <QMap>
#include <QString>

namespace QtOrm {
namespace Config {

QMap<QString, QtOrm::Mapping::ClassMapBase *> ConfigurationMap::mappedClass;

ConfigurationMap::ConfigurationMap() {}

ClassMapBase *ConfigurationMap::getMappedClass(const QString &className) {
  if (!isRegisterClass(className))
    throw Exception(
        ErrorGroup::MetaData,
        QString::fromUtf8("Class '%1' do not registed.").arg(className));

  return mappedClass.value(className);
}

bool ConfigurationMap::isRegisterClass(const QString &className) {
  return mappedClass.contains(className);
}
}
}
