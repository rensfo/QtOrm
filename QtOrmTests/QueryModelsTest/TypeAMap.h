#ifndef TYPEAMAP_H
#define TYPEAMAP_H

#include "ClassMap.h"
#include "TypeA.h"

class TypeAMap : public QtOrm::Mapping::ClassMap<TypeA> {
public:
  TypeAMap() {
    setTable("TypeA");
    setId("id");
    setMap("code");
    setMap("name");
  }
};

#endif // TYPEAMAP_H
