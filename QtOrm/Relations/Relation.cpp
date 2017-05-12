#include "Relation.h"

namespace QtOrm {
namespace Mapping {

Relation::Relation(QObject *parent) : QObject(parent) {
}

bool Relation::getSaveCascade() const {
  return saveCascade;
}

bool Relation::getDeleteCascade() const
{
  return deleteCascade;
}
}
}
