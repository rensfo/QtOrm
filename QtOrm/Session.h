#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QSqlDatabase>
#include <QVariant>



namespace QtOrm {
    class Session : public QObject{
        Q_OBJECT
    public:
        explicit Session(const QSqlDatabase &database, QObject *parent = 0);
        void insertObject(const QObject &object);
        void updateObject(const QObject &object);
        void deleteObject(const QObject &object);
        template<class T>
        T getById(QVariant id);

        QSqlDatabase getDatabase() const;
        void setDatabase(const QSqlDatabase &database);

    signals:

    public slots:

    private:
        QSqlDatabase database;
    };

    template<class T>
    T Session::getById(QVariant id)
    {

    }
}
#endif // SESSION_H
