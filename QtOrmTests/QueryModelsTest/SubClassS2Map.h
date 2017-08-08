#ifndef SUBCLASSS2MAP_H
#define SUBCLASSS2MAP_H

#include "SubClassS2.h"
#include "SingleTableInheritanceMap.h"

class SubClassS2Map : public QtOrm::Mapping::SingleTableInheritanceMap<SuperClassS, SubClassS2>
{
public:
  SubClassS2Map()
  {
    map("strVal", "str_val");
    setDiscrimanatorValue(2);
  }
};

#endif // SUBCLASSS2MAP_H
