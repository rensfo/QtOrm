#include "OneToOne.h"

namespace QtOrm {
namespace Mapping {

OneToOne::OneToOne() {
}

QString OneToOne::getProperty() const {
  return property;
}

OneToOne &OneToOne::setProperty(const QString &property) {
  this->property = property;
  return *this;
}

QString OneToOne::getValueProperty() const {
  return valueProperty;
}

OneToOne &OneToOne::setValueProperty(const QString &valueProperty) {
  this->valueProperty = valueProperty;
  return *this;
}
}
}
