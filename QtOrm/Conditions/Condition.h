#ifndef CONDITION_H
#define CONDITION_H

#include <QSharedPointer>
#include <QVariant>

#include "Operation.h"

namespace QtOrm {
namespace Sql {

class Condition {
public:
  Condition();
  Condition(const QString &property, const QVariant &value);
  Condition(const QString &property, const QVariantList &values);
  Condition(const Condition &other);
  virtual ~Condition();

  QVariantList getValues() const;
  void setValues(const QVariantList &value);
  void setValue(const QVariant &value);
  void clearValues();

//  QString getColumn() const;
//  void setColumn(const QString &value);

  virtual QString toSqlString(const QString &tableName, const QString &placeholder) const = 0;
  virtual QSharedPointer<Condition> clone() = 0;

  QString getProperty() const;
  void setProperty(const QString &value);

protected:
  template<typename T>
  QSharedPointer<Condition> cloneBase();

protected:
  QString property;
//  QString column;
  QVariantList values;
};

template<typename T>
QSharedPointer<Condition> Condition::cloneBase(){
  return QSharedPointer<T>::create(this->getProperty(), this->getValues());
};

}
}
#endif // CONDITION_H
