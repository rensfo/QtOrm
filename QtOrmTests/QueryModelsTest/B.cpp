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

QSharedPointer<A> B::getA() const{
  return a;
}

void B::setA(const QSharedPointer<A> &value){
  a = value;
  emit aChanged();
}
