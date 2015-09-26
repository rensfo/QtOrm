#ifndef SQLMANAGERBASE_H
#define SQLMANAGERBASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "ConfigurateMap.h"

namespace QtOrm{
    namespace Sql{
        class SqlManagerBase : public QObject
        {
            Q_OBJECT
        public:
            explicit SqlManagerBase(QSqlDatabase &database, QObject *parent = 0);
            virtual QSqlQuery getObjectById(const QString objectName, QVariant id);
            virtual QString getSelect(const QString objectName) const;
            virtual QString getFrom(const QString objectName) const;
            virtual QString getWhere(const QString objectName) const;

        private:
            QString generateTableAlias();
            void resetTableNumber();

        signals:

        public slots:

        private:
            int tableNumber;
            QString sqlQueryTemplate = "%1 %2 %3";
            QSqlDatabase database;
        };
    }
}
#endif // SQLMANAGERBASE_H
