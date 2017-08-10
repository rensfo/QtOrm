#ifndef CONFIGURATIONMAP_H
#define CONFIGURATIONMAP_H

#include <QMap>
#include <QSharedPointer>
#include <type_traits>

#include "ClassMapBase.h"
#include "SubClassMap.h"

namespace QtOrm {
namespace Config {
using namespace QtOrm::Mapping;

class ConfigurationMap {
public:
  ConfigurationMap();
  template <typename T>
  void addMapping();
  template <typename... Args>
  void addMappings();
  template <typename T>
  void removeMapping();
  void removeAllMappings();
  QSharedPointer<ClassMapBase> getMappedClass(const QString &className);
  QSharedPointer<ClassMapBase> getMappedClass(const QSharedPointer<QObject> &object);
  bool isRegisterClass(const QString &className);
  QStringList getRegistredClasses();
  template <typename T>
  void checkParent();

protected:
  template <class ... Args>
  typename std::enable_if<sizeof...(Args) == 0>::type unpacking();

  template <typename T, typename... Args>
  void unpacking();

protected:
  QMap<QString, QSharedPointer<ClassMapBase>> mappedClass;
  QMultiMap<QString, QSharedPointer<ClassMapBase>> derrivedClasses;
};

template<typename... Args>
void ConfigurationMap::addMappings() {
  unpacking<Args...>();
}

template <typename... Args>
typename std::enable_if<sizeof...(Args) == 0>::type ConfigurationMap::unpacking() {
}

template <typename T, typename... Args>
void ConfigurationMap::unpacking() {
  addMapping<T>();
  unpacking<Args...>();
}

template <typename T>
void ConfigurationMap::checkParent() {
  static_assert(std::is_base_of<ClassMapBase, T>(), "Type was not inherited from ClassMapBase");
}

template <typename T>
void ConfigurationMap::addMapping() {
  checkParent<T>();

  QSharedPointer<T> classMap = QSharedPointer<T>::create();
  QString className = classMap->getClassName();
  mappedClass.insert(className, classMap);
  if(derrivedClasses.contains(className)) {
    auto classes = derrivedClasses.values(className);
    classMap->appendDerrivedClass(classes);
    for(auto c : classes){
      c->toSubclass()->setSuperClass(classMap);
    }
    derrivedClasses.remove(className);
  }
  if(classMap->isSubclass()) {
    auto subClass = classMap->toSubclass();
    auto superClassName = subClass->getSuperClassName();
    if(mappedClass.contains(superClassName)){
      auto super = mappedClass.value(superClassName);
      super->appendDerrivedClass(classMap.template objectCast<ClassMapBase>());
      subClass->setSuperClass(super);
    } else {
      derrivedClasses.insert(superClassName, classMap);
    }
  }
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
