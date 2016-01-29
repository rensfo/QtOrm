#ifndef NULLABLEBASE_H
#define NULLABLEBASE_H

#include <QObject>


class NullableBase : public QObject
{
    Q_OBJECT
public:
    NullableBase(QObject *parent = 0);
    virtual bool hasValue() const;
    virtual QVariant getVariant() const = 0;

protected:
    bool isNull;
};

Q_DECLARE_METATYPE(NullableBase)

#endif // NULLABLEBASE_H
