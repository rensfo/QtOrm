#ifndef ONETOMANY_H
#define ONETOMANY_H

#include <QString>

namespace QtOrm {
namespace Mapping {

class OneToMany {
public:
  OneToMany();

  QString getProperty() const;
  OneToMany &setProperty(const QString &property);

  QString getRefClass() const;
  OneToMany &setRefClass(const QString &refClass);

  QString getRefProperty() const;
  OneToMany &setRefProperty(const QString &refProperty);

private:
  QString property;
  QString refClass;
  QString refProperty;
};
}
}
#endif // ONETOMANY_H
