#ifndef EMAP_H
#define EMAP_H

#include "ClassMap.h"
#include "E.h"

class EMap : public QtOrm::Mapping::ClassMap<E> {
public:
  EMap() {
    setTable("E");
    setId("id");

    setOneToOne("c").setTableColumn("idC");
    setOneToOne("d").setTableColumn("idD");
  }
};

#endif // EMAP_H
