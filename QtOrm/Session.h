#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QVariant>
#include <QList>

#include "ConfigurateMap.h"
#include "Exception.h"
#include "SimpleSqlBuilder.h"

namespace QtOrm {
    class Session : public QObject{
        Q_OBJECT
    public:
        explicit Session(const QSqlDatabase &database, Sql::SqlBuilderType sqlManagerType, QObject *parent = 0);
        explicit Session(const QSqlDatabase &database, QTextStream *textStream, Sql::SqlBuilderType sqlManagerType, QObject *parent = 0);
        void insertObject(const QObject &object);
        void updateObject(const QObject &object);
        void deleteObject(const QObject &object);
        template<class T>
        T *getById(const QVariant &id);
        template<class T>
        QList<T*> *getList();
        template<class T>
        QList<T*> *getList(const QString &property, const QVariant &value);

        QSqlDatabase getDatabase() const;
        void setDatabase(const QSqlDatabase &database);

    private:
        void sqlToStream(const QSqlQuery query);
        void checkClass(const QString &className);
        void queryExec(QSqlQuery &query);
        void noDataFoundCheck(const QSqlQuery &query);
        void tooManyRowsCheck(const QSqlQuery &query, const QString &id);
        QList<QObject*> *getList(const QString className, const QString &property, const QVariant &value);
        void fillObject(const QMap<QString, Mapping::PropertyMap*> &properties, const QSqlRecord &record, QObject &object);
        void fillOneToMany(const QMap<QString, Mapping::OneToMany *> &relations, const QString &idProperty, QObject &object);

    private:
        QSqlDatabase database;
        Sql::SqlBuilderBase *sqlBuilder;
        QTextStream *textStream;
    };

    template<class T>
    T *Session::getById(const QVariant &id)
    {
        QString className = T::staticMetaObject.className();
        checkClass(className);
        Mapping::ClassMapBase *classMap = Config::ConfigurateMap::getMappedClass(className);
        QList<T*> *list = getList<T>(classMap->getIdProperty().getName(), id);

        if(database.driver()->hasFeature(QSqlDriver::QuerySize) && list->size() == 0)
            throw new Exception("Не найдено ни одной записи.");

        if(database.driver()->hasFeature(QSqlDriver::QuerySize) && list->size() > 1)
            throw new Exception(QString("Найдено %1 записей с идентификатором '%2'").arg(list->size()).arg(id.toString()));

        return list->takeFirst();
    }

    template<class T>
    QList<T*> *Session::getList(){
        QString className = T::staticMetaObject.className();
        QList<T*> *list = (QList<T*>*)getList(className, "", QVariant());
        return list;
    }

    template<class T>
    QList<T*> *Session::getList(const QString &property, const QVariant &value) {
        QString className = T::staticMetaObject.className();
        QList<T*> *list = (QList<T*>*)getList(className, property, value);
        return list;
    }
}
#endif // SESSION_H
