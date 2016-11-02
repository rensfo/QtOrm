#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QVariant>

#include "Operation.h"

class Condition : public QObject {
  Q_OBJECT
public:
  explicit Condition(QObject *parent = nullptr);
  Condition(const QString &fieldName, const Operation &operation, const QVariant &value, QObject *parent = nullptr);
  Condition(const Condition &filter);

  QString getFieldName() const;
  void setFieldName(const QString &value);

  Operation getOperation() const;
  void setOperation(const Operation &value);

  QList<QVariant> getValues() const;
  void setValues(const QList<QVariant> &value);
  void setValue(const QVariant &value);
  void clearValues();

  void addValue(const QVariant &value);

private:
  QString fieldName;
  Operation operation = Operation::None;
  QList<QVariant> values;
};

#endif // FILTER_H
