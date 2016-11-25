#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <QVariant>

#include "Operation.h"

namespace QtOrm {
namespace Sql {

class Condition : public QObject {
  Q_OBJECT
public:
  explicit Condition(QObject *parent = nullptr);
  Condition(const QString &propertyName, const Operation &operation, const QVariant &value, QObject *parent = nullptr);
  Condition(const Condition &filter);

  QString getPropertyName() const;
  void setPropertyName(const QString &value);

  Operation getOperation() const;
  void setOperation(const Operation &value);

  QList<QVariant> getValues() const;
  void setValues(const QList<QVariant> &value);
  void setValue(const QVariant &value);
  void clearValues();

  void addValue(const QVariant &value);

private:
  QString propertyName;
  Operation operation = Operation::None;
  QList<QVariant> values;
};

}
}
#endif // CONDITION_H
