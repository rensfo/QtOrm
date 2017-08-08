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
};

template<typename SUPER, typename T>
ClassTableInheritanceMap<SUPER, T>::ClassTableInheritanceMap() : ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>(){
  this->setInheritanceType(InheritanceType::ClassTable);
  this->setSuperClass(ConfigurationMap::getMappedClass(SUPER::staticMetaObject.className()));
}

template<typename SUPER, typename T>
ClassTableInheritanceMap<SUPER, T>::~ClassTableInheritanceMap(){
}

}
}

#endif // CLASSTABLEINHERITANCEMAP_H
