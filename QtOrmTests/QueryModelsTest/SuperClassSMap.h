#ifndef SUPERCLASSSMAP_H
#define SUPERCLASSSMAP_H

#include "ClassMap.h"
#include "SuperClassS.h"

class SuperClassSMap : public QtOrm::Mapping::ClassMap<SuperClassS>
{
public:
  SuperClassSMap()
  {
    setTable("super_class_s");
    id("id");
    map("code");
    discriminator("discriminator", "type");
  }
};

#endif // SUPERCLASSSMAP_H
