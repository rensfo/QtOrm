#ifndef SUBCLASSS1MAP_H
#define SUBCLASSS1MAP_H

#include "SubClassS1.h"
#include "Mappings/SingleTableInheritanceMap.h"

class SubClassS1Map : public QtOrm::Mapping::SingleTableInheritanceMap<SuperClassS, SubClassS1>
{
public:
  SubClassS1Map()
  {
    setMap("intVal", "int_val").setNull(0);
    setDiscrimanitorValue(1);
  }
};

#endif // SUBCLASSS1MAP_H
