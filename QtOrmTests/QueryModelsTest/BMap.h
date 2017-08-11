#ifndef BMAP_H
#define BMAP_H

#include "B.h"
#include "Mappings/ClassMap.h"

class BMap : public QtOrm::Mapping::ClassMap<B> {
public:
  BMap() {
    setTable("B");
    setId("id");
    map("code");

    addOneToOne("a").setTableColumn("idA");
  }
};

#endif // BMAP_H
