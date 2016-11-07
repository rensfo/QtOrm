#include "E.h"

E::E(QObject *parent) : QObject(parent) {
}

C *E::getC() const {
  return c;
}

void E::setC(C *value) {
  c = value;
  emit cChanged();
}

D *E::getD() const {
  return d;
}

void E::setD(D *value) {
  d = value;
  emit dChanged();
}

long E::getId() const {
  return id;
}

void E::setId(long value) {
  id = value;
}
