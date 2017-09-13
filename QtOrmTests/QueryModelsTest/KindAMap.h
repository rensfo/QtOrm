#ifndef KINDAMAP_H
#define KINDAMAP_H

#include "Mappings/ClassMap.h"
#include "KindA.h"

class KindAMap : public QtOrm::Mapping::ClassMap<KindA> {
public:
  KindAMap() {
    setTable("KindA");
    mapId("id");
    map("code");
    map("name");
    addOneToOne("typeA").setTableColumn("idTypeA");
  }
};

Q_DECLARE_METATYPE(QSharedPointer<KindA>);

#endif // KINDAMAP_H
