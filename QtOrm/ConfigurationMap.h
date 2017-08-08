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
  static void addMapping();
  template <typename... Args>
  static void addMappings();
  template <typename T>
  static void removeMapping();
  static void removeAllMappings();
  static QSharedPointer<ClassMapBase> getMappedClass(const QString &className);
  static QSharedPointer<ClassMapBase> getMappedClass(const QSharedPointer<QObject> &object);
  static bool isRegisterClass(const QString &className);
  static QStringList getRegistredClasses();
  template <typename T>
  static void checkParent();
  static QList<QSharedPointer<ClassMapBase> > getDerrivedClasses(const QString& value);
  static bool isBaseClass(const QString& value);
  static bool isBaseClass(const QSharedPointer<ClassMapBase>& value);
  static InheritanceType getInheritanceType(const QSharedPointer<ClassMapBase>& value);

protected:
  template <class ... Args>
  static typename std::enable_if<sizeof...(Args) == 0>::type unpacking();

  template <typename T, typename... Args>
  static void unpacking();

protected:
  static QMap<QString, QSharedPointer<ClassMapBase>> mappedClass;
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
