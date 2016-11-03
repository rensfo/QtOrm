#include "Exception.h"

namespace QtOrm {
Exception::Exception(ErrorCode code, const QString &message) : QException(), code(code), message(message) {
}

Exception::Exception(const Exception &other) : Exception(other.getCode(), other.getMessage()) {
}

void Exception::raise() const
{
  throw *this;
}

QException *Exception::clone() const
{
  return new Exception(*this);
}

QString Exception::getMessage() const {
  return message;
}

ErrorCode Exception::getCode() const {
  return code;
}
}
