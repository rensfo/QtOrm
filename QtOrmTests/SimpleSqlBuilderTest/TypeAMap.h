#ifndef TYPEAMAP_H
#define TYPEAMAP_H

#include "ClassMap.h"
#include "TypeA.h"

class TypeAMap : public QtOrm::Mapping::ClassMap<TypeA> {
public:
  TypeAMap() {
    setTable("TypeA");
    id("id");
    map("code");
    map("name");
  }
};

#endif // TYPEAMAP_H
