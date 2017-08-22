#ifndef SUBCLASSS1CTIMAP_H
#define SUBCLASSS1CTIMAP_H

#include "SubClassS1.h"
#include "Mappings/ClassTableInheritanceMap.h"

class SubClassS1CtiMap : public QtOrm::Mapping::ClassTableInheritanceMap<SuperClassS, SubClassS1>
{
public:
  SubClassS1CtiMap()
  {
    setDiscrimanitorValue(1);
    setTable("sub_class_s1");
    mapId("id", "idS1");
    map("intVal", "int_val").setNull(0);
  }
};

#endif // SUBCLASSS1CTIMAP_H
