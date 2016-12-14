#include "A.h"

A::A(QObject *parent) : QObject(parent) {
}

A::~A(){
}

long A::getId() const {
  return id;
}

void A::setId(long value) {
  id = value;
}

QString A::getCode() const {
  return code_1;
}

void A::setCode(const QString &value) {
  code_1 = value;
  emit codeChanged();
}

QList<QSharedPointer<B>> A::getChild() const {
  return child;
}

void A::setChild(const QList<QSharedPointer<B> > &value) {
  child = value;
}

QSharedPointer<KindA> A::getKindA() const {
    return kindA;
}

void A::setKindA(QSharedPointer<KindA> value) {
  kindA = value;
  emit kindAChanged();
}
