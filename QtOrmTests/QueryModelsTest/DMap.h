#ifndef DMAP_H
#define DMAP_H

#include "ClassMap.h"
#include "D.h"

class DMap : public QtOrm::Mapping::ClassMap<D> {
public:
  DMap() {
    setTable("D");
    id("id");

    oneToOne("kindA").setTableColumn("idKindA");
  }
};

#endif // DMAP_H
