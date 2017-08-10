#ifndef KINDAMAP_H
#define KINDAMAP_H

#include "ClassMap.h"
#include "KindA.h"

class KindAMap : public QtOrm::Mapping::ClassMap<KindA> {
public:
  KindAMap() {
    setTable("KindA");
    setId("id");
    setMap("code");
    setMap("name");
    setOneToOne("typeA").setTableColumn("idTypeA");
  }
};

Q_DECLARE_METATYPE(QSharedPointer<KindA>);

#endif // KINDAMAP_H
