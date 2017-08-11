#ifndef EMAP_H
#define EMAP_H

#include "Mappings/ClassMap.h"
#include "E.h"

class EMap : public QtOrm::Mapping::ClassMap<E> {
public:
  EMap() {
    setTable("E");
    setId("id");

    addOneToOne("c").setTableColumn("idC");
    addOneToOne("d").setTableColumn("idD");
  }
};

#endif // EMAP_H
