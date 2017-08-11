#ifndef AMAP_H
#define AMAP_H

#include "A.h"
#include "Mappings/ClassMap.h"

class AMap : public QtOrm::Mapping::ClassMap<A> {
public:
  AMap() {
  setTable("A");
  setId("id");
  map("code_1").setColumn("code");

  addOneToOne("kindA").setTableColumn("idKindA");

  addOneToMany("child").setColumn("idA").setSaveCascade(true).setOrderBy("code", Sort::DESC);
  }
};

#endif // AMAP_H
