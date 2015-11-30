#ifndef CONFIGURATEMAP_H
#define CONFIGURATEMAP_H

#include <QMap>
#include <QDebug>

#include "ClassMap.h"
#include "ClassMapBase.h"

namespace QtOrm {
namespace Config {
using namespace QtOrm::Mapping;

class ConfigurateMap {
public:
  ConfigurateMap();
  template <class T> static void classRegister();
  static ClassMapBase *getMappedClass(QString className);
  static bool isRegisterClass(QString className);
  static void writeRegisteredClassesToDebug();
};

extern QMap<QString, ClassMapBase *> mappedClass;

template <class T> void ConfigurateMap::classRegister() {
  (void)static_cast<ClassMapBase *>((T *)0);

  T *classMap = new T();
  mappedClass.insert(classMap->getClassName(), classMap);
}
}
}

#endif // CONFIGURATEMAP_H
