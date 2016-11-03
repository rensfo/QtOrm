#ifndef AMAP_H
#define AMAP_H

#include "A.h"
#include "ClassMap.h"

class AMap : public QtOrm::Mapping::ClassMap<A> {
public:
  AMap() {
    setTable("A");
    id("id");
    map("code");
    oneToOne("kindA").setTableColumn("idKindA");
    oneToMany("child").setRefClass("B").setRefProperty("idA");
  }
};

#endif // AMAP_H
