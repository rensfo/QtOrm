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
  template <typename T>
  static void addMapping();
  template <typename T>
  static void removeMapping();
  static QSharedPointer<ClassMapBase> getMappedClass(const QString &className);
  static QSharedPointer<ClassMapBase> getMappedClass(const QSharedPointer<QObject> &object);
  static bool isRegisterClass(const QString &className);
  static QStringList getRegistredClasses();
  template <typename T>
  static void checkParent();

protected:
  static QMap<QString, QSharedPointer<ClassMapBase>> mappedClass;
};

template<typename T>
void ConfigurationMap::checkParent() {
  static_assert(std::is_base_of<ClassMapBase, T>(), "Type was not inherited from ClassMapBase");
}

template <typename T>
void ConfigurationMap::addMapping() {
  checkParent<T>();

  QSharedPointer<T> classMap = QSharedPointer<T>::create();
  mappedClass.insert(classMap->getClassName(), classMap);
}

template <typename T>
void ConfigurationMap::removeMapping() {
  checkParent<T>();

  T classMap;
  QString className = classMap.getClassName();
  mappedClass.value(className)->deleteLater();
  mappedClass.remove(className);
}
}
}

#endif // CONFIGURATIONMAP_H
