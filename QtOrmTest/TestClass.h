#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>

class TestClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getId WRITE setId)
    Q_PROPERTY(QString name READ getName WRITE setName)

public:
    explicit TestClass(QObject *parent = 0);
    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);


signals:

public slots:

private:
    int id;
    QString name;
};

#endif // TESTCLASS_H
