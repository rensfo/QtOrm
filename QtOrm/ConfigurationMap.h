#ifndef CONFIGURATIONMAP_H
#define CONFIGURATIONMAP_H

#include <QMap>
#include <QSharedPointer>

#include "ClassMapBase.h"

namespace QtOrm {
namespace Config {
using namespace QtOrm::Mapping;

class ConfigurationMap {
public:
  ConfigurationMap();
  template <class T>
  static void addMapping();
  template <class T>
  static void removeMapping();
  static QSharedPointer<ClassMapBase> getMappedClass(const QString &className);
  static bool isRegisterClass(const QString &className);

private:
  static QMap<QString, QSharedPointer<ClassMapBase>> mappedClass;
};

template <class T>
void ConfigurationMap::addMapping() {

  (void)static_cast<ClassMapBase *>((T *)0);

  QSharedPointer<T> classMap = QSharedPointer<T>::create();
  mappedClass.insert(classMap->getClassName(), classMap);
}

template <class T>
void ConfigurationMap::removeMapping() {
  (void)static_cast<ClassMapBase *>((T *)0);

  T classMap;
  QString className = classMap.getClassName();
  mappedClass.value(className)->deleteLater();
  mappedClass.remove(className);
}
}
}

#endif // CONFIGURATIONMAP_H
