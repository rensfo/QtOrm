#ifndef SUBCLASSS2CTIMAP_H
#define SUBCLASSS2CTIMAP_H

#include "SubClassS2.h"
#include "ClassTableInheritanceMap.h"

class SubClassS2CtiMap : public QtOrm::Mapping::ClassTableInheritanceMap<SuperClassS, SubClassS2>
{
public:
  SubClassS2CtiMap()
  {
    setDiscrimanitorValue(2);
    setTable("sub_class_s2");
    id("id", "idS2").setAutoincrement(false);
    map("strVal", "str_val");
  }
};

#endif // SUBCLASSS2CTIMAP_H
