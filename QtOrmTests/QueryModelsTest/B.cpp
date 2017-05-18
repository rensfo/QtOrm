#include "B.h"

#include "A.h"

B::B(QObject *parent) : QObject(parent) {
}

B::~B() {
}

long B::getId() const {
  return id;
}

void B::setId(long value) {
  id = value;
}

QString B::getCode() const {
  return code;
}

void B::setCode(const QString &value) {
  code = value;
}

QWeakPointer<A> B::getA() const {
  return a;
}

void B::setA(const QWeakPointer<A> &value) {
  a = value;
  emit aChanged();
}
