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

}
}

#endif // CLASSTABLEINHERITANCEMAP_H
