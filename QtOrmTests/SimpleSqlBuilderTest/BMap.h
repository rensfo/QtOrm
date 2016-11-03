#ifndef BMAP_H
#define BMAP_H

#include "B.h"
#include "ClassMap.h"

class BMap : public QtOrm::Mapping::ClassMap<B> {
public:
  BMap() {
    setTable("B");
    id("id");
    map("idA");
    map("code");
  }
};

#endif // BMAP_H
