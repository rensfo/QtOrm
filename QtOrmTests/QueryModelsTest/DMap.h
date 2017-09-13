#ifndef DMAP_H
#define DMAP_H

#include "Mappings/ClassMap.h"
#include "D.h"

class DMap : public QtOrm::Mapping::ClassMap<D> {
public:
  DMap() {
    setTable("D");
    mapId("id");

    addOneToOne("kindA").setTableColumn("idKindA");
  }
};

#endif // DMAP_H
