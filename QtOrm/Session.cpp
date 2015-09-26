#include "Session.h"

namespace QtOrm { 
    Session::Session(const QSqlDatabase &database, QObject *parent)
        : QObject(parent), database(database) {
        sqlManager = new Sql::SimpleSqlManager(database);
    }

    void Session::insertObject(const QObject &object) {

    }

    void Session::updateObject(const QObject &object) {

    }

    void Session::deleteObject(const QObject &object) {

    }

    QSqlDatabase Session::getDatabase() const {
        return database;
    }

    void Session::setDatabase(const QSqlDatabase &database) {
        this->database = database;
    }

}

