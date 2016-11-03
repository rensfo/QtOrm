#include "Exception.h"

namespace QtOrm {
Exception::Exception(ErrorGroup code, const QString &message, QObject *parent)
    : QObject(parent), group(code), message(message) {
}

Exception::Exception(const Exception &exception) : QObject(nullptr) {
  setParent(exception.parent());
  group = exception.getGroup();
  message = exception.getMessage();
}

QString Exception::getMessage() const {
  return message;
}

ErrorGroup Exception::getGroup() const {
  return group;
}
}
