#include "Exception.h"

namespace QtOrm {
Exception::Exception(const QString &message) : QException(), message(message) {
}

Exception::Exception(const Exception &other) : Exception(other.getMessage()) {
}

void Exception::raise() const {
  throw *this;
}

QException *Exception::clone() const {
  return new Exception(*this);
}

QString Exception::getMessage() const {
  return message;
}
}
