#ifndef SQLMANAGERBASE_H
#define SQLMANAGERBASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "ConfigurateMap.h"

namespace QtOrm{
    namespace Sql{
        enum class SqlManagerType{ Simple, Setter };

        class SqlManagerBase : public QObject
        {
            Q_OBJECT
        public:
            explicit SqlManagerBase(const QSqlDatabase &database, QObject *parent = 0);
            virtual QSqlQuery getObjectById(const QString objectName, QVariant id) = 0;
            virtual QSqlQuery insertObject(const QObject &object) = 0;
            virtual QSqlQuery updateObject(const QObject &object) = 0;
            virtual QSqlQuery deleteObject(const QObject &object) = 0;

        protected:
            QString generateTableAlias();
            void resetTableNumber();

        signals:

        public slots:

        protected:
            int tableNumber;
            QString sqlQueryTemplate = "%1 %2 %3";
            QSqlDatabase database;

        };
    }
}
#endif // SQLMANAGERBASE_H
