#ifndef EMAP_H
#define EMAP_H

#include "Mappings/ClassMap.h"
#include "E.h"

class EMap : public QtOrm::Mapping::ClassMap<E> {
public:
  EMap() {
    setTable("E");
    mapId("id");

    addOneToOne("c").setTableColumn("idC");
    addOneToOne("d").setTableColumn("idD");
  }
};

#endif // EMAP_H
