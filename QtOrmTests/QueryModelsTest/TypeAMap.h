#ifndef TYPEAMAP_H
#define TYPEAMAP_H

#include "Mappings/ClassMap.h"
#include "TypeA.h"

class TypeAMap : public QtOrm::Mapping::ClassMap<TypeA> {
public:
  TypeAMap() {
    setTable("TypeA");
    setId("id");
    map("code");
    map("name");
  }
};

#endif // TYPEAMAP_H
