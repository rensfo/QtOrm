#include "Session.h"

namespace QtOrm { 
    Session::Session(const QSqlDatabase &database, Sql::SqlBuilderType sqlManagerType, QObject *parent)
        : Session(database, nullptr, sqlManagerType, parent) {
    }

    Session::Session(const QSqlDatabase &database, QTextStream *textStream, Sql::SqlBuilderType sqlManagerType, QObject *parent)
        : QObject(parent), database(database), textStream(textStream){
        //Должна быть фабрика, если потребуется
        switch(sqlManagerType){
        case Sql::SqlBuilderType::Simple:
            sqlBuilder = new Sql::SimpleSqlBuilder(database);
            break;
        case Sql::SqlBuilderType::Setter:
            break;
        }
    }

    void Session::insertObject(const QObject &object) {
        QSqlQuery query = sqlBuilder->insertObject(object);
        if(!query.exec()){
            sqlToStream(query);
            throw new Exception(query.lastError().text());
        }

        sqlToStream(query);
    }

    void Session::updateObject(const QObject &object) {
        QSqlQuery query = sqlBuilder->updateObject(object);
        if(!query.exec()){
            sqlToStream(query);
            throw new Exception(query.lastError().text());
        }

        sqlToStream(query);
    }

    void Session::deleteObject(const QObject &object) {
        QSqlQuery query = sqlBuilder->deleteObject(object);
        if(!query.exec()){
            sqlToStream(query);
            throw new Exception(query.lastError().text());
        }
        sqlToStream(query);
    }

    QSqlDatabase Session::getDatabase() const {
        return database;
    }

    void Session::setDatabase(const QSqlDatabase &database) {
        this->database = database;
    }

    void Session::sqlToStream(const QSqlQuery query) {
        if(textStream){
            *textStream << query.lastQuery() << endl;

            auto boundValues = query.boundValues();
            for(auto it = boundValues.begin(); it != boundValues.end(); ++it)
                *textStream << it.key() << " = " << it.value().toString() << endl;
        }
    }

    void Session::checkClass(const QString &className) {
        if(!Config::ConfigurateMap::isRegisterClass(className))
            throw new Exception(QString("Класс '%1' не зарегистрирован.").arg(className));
    }

    void Session::queryExec(QSqlQuery &query) {
        if(!query.exec()){
            sqlToStream(query);
            throw new Exception(query.lastError().text());
        }
        sqlToStream(query);
    }

    void Session::noDataFoundCheck(const QSqlQuery &query) {
        if(database.driver()->hasFeature(QSqlDriver::QuerySize) && query.size() == 0)
            //throw new Exception(QString("Нет записи с идентификатором '%1'").arg(id.toString()));
            throw new Exception("Не найдено ни одной записи.");
    }

    void Session::tooManyRowsCheck(const QSqlQuery &query, const QString &id) {
        if(database.driver()->hasFeature(QSqlDriver::QuerySize) && query.size() > 1)
            throw new Exception(QString("Найдено %1 записей с идентификатором '%2'").arg(query.size()).arg(id));
    }

    QList<QObject *> *Session::getList(const QString className, const QString &property, const QVariant &value) {
        int id = QMetaType::type(className.toStdString().data());
        if (id == -1)
            throw new Exception(QString("Класс %1 не найден.").arg(className));

        checkClass(className);

        QSqlQuery query = sqlBuilder->getListObject(className, property, value);
        queryExec(query);

        Mapping::ClassMapBase *classMap = Config::ConfigurateMap::getMappedClass(className);
        auto properties = classMap->getProperties();
        QList<QObject *> *objects = new QList<QObject *>();
        while(query.next()) {
            QObject *obj = classMap->getMetaObject().newInstance();
            fillObject(properties, query.record(), *obj);
            fillOneToMany(classMap->getOneToManyRelations(), classMap->getIdProperty().getName(), *obj);
            objects->append(obj);
        }

        return objects;

    }

    void Session::fillObject(const QMap<QString, Mapping::PropertyMap *> &properties, const QSqlRecord &record, QObject &object){
        foreach(auto prop, properties) {
            QVariant value = record.value(prop->getColumn());
            object.setProperty(prop->getName().toStdString().c_str(), value);
        }
    }

    void Session::fillOneToMany(const QMap<QString, Mapping::OneToMany *> &relations, const QString &idProperty, QObject &object) {
        foreach(auto oneToMany, relations) {
            QString refClass = oneToMany->getRefClass();
            QString property = oneToMany->getProperty();
            QString refProperty = oneToMany->getRefProperty();
            QVariant value = object.property(idProperty.toStdString().data());
            QList<QObject*> *l = getList(refClass, refProperty, value);
            QVariant var = QVariant::fromValue(*l);
            if(!object.setProperty(property.toStdString().data(), var)){
                QString textError = QString("Произошла ошибка при присваивании значения  '%1' свойству '%2.%3'")
                        .arg(var.toString())
                        .arg(object.metaObject()->className())
                        .arg(property);
                throw new Exception(textError);
            }
        }
    }

}

