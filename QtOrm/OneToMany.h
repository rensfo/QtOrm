#ifndef ONETOMANY_H
#define ONETOMANY_H

#include "Relation.h"

#include <QString>

namespace QtOrm {
namespace Mapping {

class OneToMany : public Relation {
  Q_OBJECT
public:
  explicit OneToMany(QObject *parent = nullptr);

  QString getProperty() const;
  OneToMany &setProperty(const QString &property);

  QString getRefClass() const;
  OneToMany &setRefClass(const QString &refClass);

  QString getRefProperty() const;
  OneToMany &setRefProperty(const QString &refProperty);

  OneToMany &setSaveCascade(bool value);

private:
  QString property;
  QString refClass;
  QString refProperty;
};
}
}
#endif // ONETOMANY_H
