#ifndef BMAP_H
#define BMAP_H

#include "B.h"
#include "ClassMap.h"

class BMap : public QtOrm::Mapping::ClassMap<B> {
public:
  BMap() {
    setTable("B");
    setId("id");
    setMap("code");

    setOneToOne("a").setTableColumn("idA");
  }
};

#endif // BMAP_H
