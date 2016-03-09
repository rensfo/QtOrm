#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QVariant>

enum class Operation { Like, Equal, NotEqual };

class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(QObject *parent = 0);
    Filter(const Filter &filter);

    QString getFieldName() const;
    void setFieldName(const QString &value);

    Operation getOperation() const;
    void setOperation(const Operation &value);

    QList<QVariant> getValues() const;
    void setValues(const QList<QVariant> &value);
    void clearValues();

    void addValue(const QVariant &value);

signals:

public slots:

private:
    QString fieldName;
    Operation operation;
    QList<QVariant> values;
};

#endif // FILTER_H
