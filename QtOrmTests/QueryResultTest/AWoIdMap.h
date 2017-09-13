#ifndef AWOID_H
#define AWOID_H

#include "A.h"
#include "Mappings/ClassMap.h"

class AWoIdMap : public QtOrm::Mapping::ClassMap<A> {
public:
  AWoIdMap() {
    setTable("A");
    setId("id");
    map("code_1", "code");
  }
};

#endif // AWOID_H
