#include "C.h"

C::C(QObject *parent) : QObject(parent) {
}

C::~C(){
}

long C::getId() const {
  return id;
}

void C::setId(long value) {
  id = value;
}

QSharedPointer<KindA> C::getKindA() const {
  return kindA;
}

void C::setKindA(QSharedPointer<KindA> value) {
  kindA = value;
  emit kindAChanged();
}
