#ifndef KINDAMAP_H
#define KINDAMAP_H

#include "ClassMap.h"
#include "KindA.h"

class KindAMap : public QtOrm::Mapping::ClassMap<KindA> {
public:
  KindAMap() {
    setTable("KindA");
    id("id");
    map("code");
    map("name");
    oneToOne("typeA").setTableColumn("idTypeA");
  }
};

#endif // KINDAMAP_H
