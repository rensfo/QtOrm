#ifndef SQLMANAGERBASE_H
#define SQLMANAGERBASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "ConfigurateMap.h"

namespace QtOrm{
    namespace Sql{
        enum class SqlBuilderType{ Simple, Setter };

        class SqlBuilderBase : public QObject
        {
            Q_OBJECT
        public:
            explicit SqlBuilderBase(const QSqlDatabase &database, QObject *parent = 0);
            virtual QSqlQuery getListObject(const QString &objectName) = 0;
            virtual QSqlQuery getListObject(const QString &objectName, const QString property, const QVariant value) = 0;
            virtual QSqlQuery insertObject(const QObject &object) = 0;
            virtual QSqlQuery updateObject(const QObject &object) = 0;
            virtual QSqlQuery deleteObject(const QObject &object) = 0;

        protected:
            QString generateTableAlias();
            QString getCurrentTableAlias() const;
            void resetTableNumber();

        signals:

        public slots:

        protected:
            int tableNumber;
            const QString sqlQueryTemplate = "%1 %2 %3";
            const QString tableAliasTemplate = "tb_%1";
            QSqlDatabase database;

        };
    }
}
#endif // SQLMANAGERBASE_H
