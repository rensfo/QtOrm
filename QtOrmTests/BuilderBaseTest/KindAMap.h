#ifndef KINDAMAP_H
#define KINDAMAP_H

#include "ClassMap.h"
#include "KindA.h"

class KindAMap : public QtOrm::Mapping::ClassMap<KindA>
{
public:
  KindAMap()
  {
    setTable("KindA");
    id("id");
    map("code");
    map("name");
    oneToOne("typeA").setTableColumn("idTypeA");
  }
};

Q_DECLARE_METATYPE(KindA *)
Q_DECLARE_METATYPE(QList<KindA *>)

#endif // KINDAMAP_H
