#ifndef SUBCLASS3MAP_H
#define SUBCLASS3MAP_H

#include "SubClassS3.h"
#include "SubclassMap.h"

class SubClassS3Map : public QtOrm::Mapping::SubclassMap<SuperClassS, SubClassS3>
{
public:
  SubClassS3Map()
  {
    setDiscrimanatorValue(3);
    oneToOne("ref").setTableColumn("id_ref");
  }
};

#endif // SUBCLASS3MAP_H
