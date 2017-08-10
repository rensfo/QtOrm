#ifndef CMAP_H
#define CMAP_H

#include "Mappings/ClassMap.h"
#include "C.h"

class CMap : public QtOrm::Mapping::ClassMap<C> {
public:
  CMap() {
    setTable("C");
    setId("id");

    setOneToOne("kindA").setTableColumn("idKindA");
  }
};

#endif // CMAP_H
