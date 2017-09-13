#include "TypeA.h"

TypeA::TypeA(QObject *parent) : QObject(parent) {
}

TypeA::~TypeA() {
}

long TypeA::getId() const {
  return id;
}

void TypeA::setId(long value) {
  id = value;
}

QString TypeA::getCode() const {
  return code;
}

void TypeA::setCode(const QString &value) {
  code = value;
  emit codeChanged(code);
}

QString TypeA::getName() const {
  return name;
}

void TypeA::setName(const QString &value) {
  name = value;
  emit nameChanged(name);
}
