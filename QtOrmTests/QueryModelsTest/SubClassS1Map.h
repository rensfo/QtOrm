#ifndef SUBCLASSS1MAP_H
#define SUBCLASSS1MAP_H

#include "SubClassS1.h"
#include "SubclassMap.h"

class SubClassS1Map : public QtOrm::Mapping::SubclassMap<SuperClassS, SubClassS1>
{
public:
  SubClassS1Map()
  {
    map("intVal", "int_val").setNull(0);
    setDiscrimanatorValue(1);
  }
};
//Q_DECLARE_METATYPE(QList<A*>)
//Q_DECLARE_METATYPE(A*)

#endif // SUBCLASSS1MAP_H
