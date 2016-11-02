#ifndef BMAP_H
#define BMAP_H

#include "ClassMap.h"
#include "B.h"

class BMap : public QtOrm::Mapping::ClassMap<B>
{
public:
  BMap()
  {
    setTable("B");
    id("id");
    map("idA");
    map("code");
  }
};

Q_DECLARE_METATYPE(B *)
Q_DECLARE_METATYPE(QList<B *>)

#endif // BMAP_H
