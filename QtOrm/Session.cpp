#include "Session.h"

namespace QtOrm { 
    Session::Session(const QSqlDatabase &database, Sql::SqlManagerType sqlManagerType, QObject *parent)
        : Session(database, nullptr, sqlManagerType, parent) {
    }

    Session::Session(const QSqlDatabase &database, QTextStream *textStream, Sql::SqlManagerType sqlManagerType, QObject *parent)
        : QObject(parent), database(database), textStream(textStream){
        //Должна быть фабрика, если потребуется
        switch(sqlManagerType){
        case Sql::SqlManagerType::Simple:
            sqlManager = new Sql::SimpleSqlManager(database);
            break;
        case Sql::SqlManagerType::Setter:
            break;
        }
    }

    void Session::insertObject(const QObject &object) {
        QSqlQuery query = sqlManager->insertObject(object);
        if(!query.exec()){
            sqlToStream(query);
            throw new Exception(query.lastError().text());
        }

        sqlToStream(query);

        //qDebug() << query.lastQuery();
    }

    void Session::updateObject(const QObject &object) {
        QSqlQuery query = sqlManager->updateObject(object);
        if(!query.exec()){
            sqlToStream(query);
            throw new Exception(query.lastError().text());
        }

        sqlToStream(query);
        //qDebug() << query.lastQuery();
    }

    void Session::deleteObject(const QObject &object) {
        QSqlQuery query = sqlManager->deleteObject(object);
        if(!query.exec()){
            sqlToStream(query);
            throw new Exception(query.lastError().text());
        }
        sqlToStream(query);
        //qDebug() << query.lastQuery();
    }

    QSqlDatabase Session::getDatabase() const {
        return database;
    }

    void Session::setDatabase(const QSqlDatabase &database) {
        this->database = database;
    }

    void Session::sqlToStream(const QSqlQuery query)
    {
        if(textStream){
            *textStream << query.lastQuery() << endl;

            auto boundValues = query.boundValues();
            for(auto it = boundValues.begin(); it != boundValues.end(); ++it)
                *textStream << it.key() << " = " << it.value().toString() << endl;
        }
    }

}

