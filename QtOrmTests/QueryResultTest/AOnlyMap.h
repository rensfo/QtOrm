#ifndef AONLYMAP_H
#define AONLYMAP_H

#include "A.h"
#include "Mappings/ClassMap.h"

class AOnlyMap : public QtOrm::Mapping::ClassMap<A> {
public:
  AOnlyMap() {
    setTable("A");
    setId("id");
    map("code_1", "code");
  }
};

#endif // AONLYMAP_H
