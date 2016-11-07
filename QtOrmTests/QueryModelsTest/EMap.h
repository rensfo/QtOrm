#ifndef EMAP_H
#define EMAP_H

#include "ClassMap.h"
#include "E.h"

class EMap : public QtOrm::Mapping::ClassMap<E> {
public:
  EMap() {
    setTable("E");
    id("id");

    oneToOne("c").setTableColumn("idC");
    oneToOne("d").setTableColumn("idD");
  }
};

#endif // EMAP_H
