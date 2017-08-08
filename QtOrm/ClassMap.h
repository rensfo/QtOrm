#ifndef CLASSMAP_H
#define CLASSMAP_H

#include "ClassMapBaseExt.h"
#include "ClassMapBase.h"

namespace QtOrm {
namespace Mapping {

template <typename T>
class ClassMap : public ClassMapBaseExt<T, QtOrm::Mapping::ClassMapBase> {
public:
  ClassMap() : ClassMapBaseExt<T, QtOrm::Mapping::ClassMapBase>(){}
};

}
}

#endif // CLASSMAP_H
