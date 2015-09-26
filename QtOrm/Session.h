#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

#include "ConfigurateMap.h"
#include "Exception.h"
#include "SimpleSqlManager.h"

namespace QtOrm {
    class Session : public QObject{
        Q_OBJECT
    public:
        explicit Session(const QSqlDatabase &database, QObject *parent = 0);
        void insertObject(const QObject &object);
        void updateObject(const QObject &object);
        void deleteObject(const QObject &object);
        template<class T>
        T *getById(const QVariant &id);

        QSqlDatabase getDatabase() const;
        void setDatabase(const QSqlDatabase &database);

    private:
        QSqlDatabase database;
        Sql::SqlManagerBase *sqlManager;
    };

    template<class T>
    T *Session::getById(const QVariant &id)
    {
        QString className = T::staticMetaObject.className();
        if(!Config::ConfigurateMap::isRegisterClass(className))
            throw new Exception(QString("Класс '%1' не зарегистрирован.").arg(className));

        QSqlQuery query = sqlManager->getObjectById(className, id);
        if(!query.exec()){
            qDebug() << query.lastQuery();
            throw new Exception(query.lastError().text());
        }

        if(query.size() == 0)
            throw new Exception(QString("Нет записи с идентификатором '%1'").arg(id.toString()));

        if(query.size() > 1)
            throw new Exception(QString("Найдено %1 записей с идентификатором '%2'").arg(query.size()).arg(id.toString()));

        auto obj = new T();

        query.next();
        QSqlRecord record = query.record();
        Mapping::ClassMapBase *classMap = Config::ConfigurateMap::getMappedClass(className);
        auto properties = classMap->getProperties();
        foreach(auto prop, properties){
            QVariant value = record.value(prop->getColumn());
            obj->setProperty(prop->getName().toStdString().c_str(), value);
        }

        return obj;
    }
}
#endif // SESSION_H
