#ifndef SUBCLASS3MAP_H
#define SUBCLASS3MAP_H

#include "SubClassS3.h"
#include "Mappings/SingleTableInheritanceMap.h"

class SubClassS3Map : public QtOrm::Mapping::SingleTableInheritanceMap<SuperClassS, SubClassS3>
{
public:
  SubClassS3Map()
  {
    setDiscrimanitorValue(3);
    addOneToOne("ref").setTableColumn("id_ref");
  }
};

#endif // SUBCLASS3MAP_H
