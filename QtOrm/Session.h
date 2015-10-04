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
        template<class T>
        QList<T*> *convertFromSqlQueryToList(const QString &className, QSqlQuery &query);

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

        QSqlQuery query = sqlBuilder->getObjectById(className, id);
        queryExec(query);

        qDebug() << " query size = " << query.size() << " query = " << query.numRowsAffected();

        if(database.driver()->hasFeature(QSqlDriver::QuerySize)){
            noDataFoundCheck(query);
            tooManyRowsCheck(query, id.toString());
        }


        QList<T*> *list = convertFromSqlQueryToList<T>(className, query);
        qDebug() << "list count = " << list->size();

        if(list->size() == 0)
            throw new Exception("Не найдено ни одной записи.");

        return list->takeFirst();
        /*auto obj = new T();

        query.next();
        QSqlRecord record = query.record();
        Mapping::ClassMapBase *classMap = Config::ConfigurateMap::getMappedClass(className);
        auto properties = classMap->getProperties();
        foreach(auto prop, properties){
            QVariant value = record.value(prop->getColumn());
            obj->setProperty(prop->getName().toStdString().c_str(), value);
        }
        return obj;*/
    }

    template<class T>
    QList<T*> *Session::getList(){
        QString className = T::staticMetaObject.className();
        checkClass(className);

        QSqlQuery query = sqlBuilder->getListObject(className);
        queryExec(query);

        QList<T*> *list = convertFromSqlQueryToList<T>(className, query);
        return list;
    }

    template<class T>
    QList<T*> *Session::getList(const QString &property, const QVariant &value){
        QString className = T::staticMetaObject.className();
        checkClass(className);

        QSqlQuery query = sqlBuilder->getListObject(className, property, value);
        queryExec(query);

        QList<T*> *list = convertFromSqlQueryToList<T>(className, query);
        return list;
    }

    template<class T>
    QList<T*> *Session::convertFromSqlQueryToList(const QString &className, QSqlQuery &query){
        Mapping::ClassMapBase *classMap = Config::ConfigurateMap::getMappedClass(className);
        auto properties = classMap->getProperties();
        QList<T*> *list = new QList<T*>();
        while(query.next()){
            T *obj = new T();
            foreach(auto prop, properties){
                QVariant value = query.record().value(prop->getColumn());
                obj->setProperty(prop->getName().toStdString().c_str(), value);
            }

            list->append(obj);
        }

        return list;
    }
}
#endif // SESSION_H
