#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QVariant>

#include "Operation.h"

class Filter : public QObject
{
  Q_OBJECT
public:
  explicit Filter(QObject *parent = nullptr);
  Filter(const QString &fieldName, const Operation &operation, const QVariant &value, QObject *parent = 0);
  Filter(const Filter &filter);

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

Q_DECLARE_METATYPE(Filter)

#endif // FILTER_H
