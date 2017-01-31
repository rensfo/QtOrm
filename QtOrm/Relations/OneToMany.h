#ifndef ONETOMANY_H
#define ONETOMANY_H

#include <QString>
#include <QMetaObject>

#include "Relation.h"
#include "../QueryModels/OrderColumn.h"

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
  template<typename T>
  OneToMany &setRefClass();

  QString getColumn() const;
  OneToMany &setColumn(const QString &column);

  OneToMany &setSaveCascade(bool value);
  OneToMany &setOrderBy(const QString &property, Sort sort = Sort::ASC);
  OneToMany &setOrderBy(const QList<OrderColumn> &orderBy);
  QList<OrderColumn> getOrderBy();
private:
  QString property;
  QString refClass;
  QString column;
  QList<OrderColumn> orderBy;
};

template<typename T>
OneToMany &OneToMany::setRefClass(){
  return setRefClass(T::staticMetaObject.className());
}

}
}
#endif // ONETOMANY_H
