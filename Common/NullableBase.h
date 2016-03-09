#ifndef NULLABLEBASE_H
#define NULLABLEBASE_H

#include <QObject>
#include <QVariant>


class NullableBase : public QObject
{
    Q_OBJECT
public:
    NullableBase(QObject *parent = 0);
    NullableBase(const NullableBase &value);
    virtual bool hasValue() const;
    virtual QVariant getVariant() const;
    virtual void setVariant(const QVariant &value);

    NullableBase &operator=(const NullableBase &nullableBase);

protected:
    bool isNull;
    QVariant value;
};

Q_DECLARE_METATYPE(NullableBase)

#endif // NULLABLEBASE_H
