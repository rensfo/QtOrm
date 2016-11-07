#include "C.h"

C::C(QObject *parent) : QObject(parent) {
}

long C::getId() const {
  return id;
}

void C::setId(long value) {
  id = value;
}

KindA *C::getKindA() const {
  return kindA;
}

void C::setKindA(KindA *value) {
  kindA = value;
  emit kindAChanged();
}
