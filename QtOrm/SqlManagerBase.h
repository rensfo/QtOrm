#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>

class SqlManager : public QObject
{
    Q_OBJECT
public:
    explicit SqlManager(QObject *parent = 0);

signals:

public slots:
};

#endif // SQLMANAGER_H
