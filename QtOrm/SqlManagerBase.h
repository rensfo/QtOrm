#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>

class SqlManagerBase : public QObject
{
    Q_OBJECT
public:
    explicit SqlManagerBase(QObject *parent = 0);

signals:

public slots:
};

#endif // SQLMANAGER_H
