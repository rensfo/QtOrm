#ifndef SUBCLASSS3CTIMAP_H
#define SUBCLASSS3CTIMAP_H

#include "SubClassS3.h"
#include "ClassTableInheritanceMap.h"

class SubClassS3CtiMap : public QtOrm::Mapping::ClassTableInheritanceMap<SuperClassS, SubClassS3>
{
public:
  SubClassS3CtiMap()
  {
    setDiscrimanitorValue(3);
    setTable("sub_class_s3");
    setId("id", "idS3").setAutoincrement(false);
    setOneToOne("ref").setTableColumn("id_ref");
  }
};


#endif // SUBCLASSS3CTIMAP_H
