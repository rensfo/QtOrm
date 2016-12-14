#include "E.h"

E::E(QObject *parent) : QObject(parent) {
}

E::~E() {
}

QSharedPointer<C> E::getC() const {
  return c;
}

void E::setC(QSharedPointer<C> value) {
  c = value;
  emit cChanged();
}

QSharedPointer<D> E::getD() const {
  return d;
}

void E::setD(QSharedPointer<D> value) {
  d = value;
  emit dChanged();
}

long E::getId() const {
  return id;
}

void E::setId(long value) {
  id = value;
}
