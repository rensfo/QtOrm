#ifndef SUPERCLASSSMAP_H
#define SUPERCLASSSMAP_H

#include "Mappings/ClassMap.h"
#include "SuperClassS.h"

class SuperClassSMap : public QtOrm::Mapping::ClassMap<SuperClassS>
{
public:
  SuperClassSMap()
  {
    setTable("super_class_s");
    setId("id");
    setMap("code");
    setDiscriminator("discriminator", "type");
  }
};

#endif // SUPERCLASSSMAP_H
