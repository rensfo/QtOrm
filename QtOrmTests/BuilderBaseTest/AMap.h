#ifndef AMAP_H
#define AMAP_H

#include "ClassMap.h"
#include "A.h"

class AMap : public QtOrm::Mapping::ClassMap<A>
{
public:
  AMap()
  {
    setTable("A");
    id("id");
    map("code");
    oneToOne("kindA").setTableColumn("idKindA");
    oneToMany("child").setRefClass("B").setRefProperty("idA");
  }
};

Q_DECLARE_METATYPE(A*)
Q_DECLARE_METATYPE(QList<A*>)

#endif // AMAP_H
