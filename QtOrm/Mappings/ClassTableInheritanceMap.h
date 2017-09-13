#ifndef CLASSTABLEINHERITANCEMAP_H
#define CLASSTABLEINHERITANCEMAP_H

#include "ClassMapBaseExt.h"
#include "SubClassMap.h"

namespace QtOrm {
namespace Mapping {

template <typename SUPER, typename T>
class ClassTableInheritanceMap : public ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap> {
public:
  ClassTableInheritanceMap();
  ~ClassTableInheritanceMap();

  virtual QString getSuperClassName() override;

  virtual PropertyMap &mapId(QString propertyName) override;
  virtual PropertyMap &mapId(const QString &propertyName, const QString &columnName) override;
};

template<typename SUPER, typename T>
ClassTableInheritanceMap<SUPER, T>::ClassTableInheritanceMap() : ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>(){
  this->setInheritanceType(InheritanceType::ClassTable);
}

template<typename SUPER, typename T>
ClassTableInheritanceMap<SUPER, T>::~ClassTableInheritanceMap(){
}

template<typename SUPER, typename T>
QString ClassTableInheritanceMap<SUPER, T>::getSuperClassName() {
  return SUPER::staticMetaObject.className();
}

template<typename SUPER, typename T>
PropertyMap& ClassTableInheritanceMap<SUPER, T>::mapId(QString propertyName) {
  return QtOrm::Mapping::SubClassMap::mapId(propertyName).setAutoincrement(false);
}

template<typename SUPER, typename T>
PropertyMap&ClassTableInheritanceMap<SUPER, T>::mapId(const QString&propertyName, const QString&columnName) {
  return QtOrm::Mapping::SubClassMap::mapId(propertyName, columnName);
}

}
}

#endif // CLASSTABLEINHERITANCEMAP_H
