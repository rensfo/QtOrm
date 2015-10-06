#ifndef SIMPLESQLMANAGER_H
#define SIMPLESQLMANAGER_H

#include "SqlBuilderBase.h"

namespace QtOrm{
    namespace Sql {
        class SimpleSqlBuilder : public SqlBuilderBase
        {
            Q_OBJECT

        public:
            SimpleSqlBuilder(const QSqlDatabase &database, QObject *parent = 0);
            QSqlQuery getListObject(const QString &objectName);
            QSqlQuery getListObject(const QString &objectName, const QString property, const QVariant value);
            QString getSelect() const;
            QString getFrom(const QString &tableName) const;
            QString getWhere(const QString &tableAlias, const QString &column, const QString &placeHolder) const;

            QSqlQuery insertObject(const QObject &object);
            QSqlQuery updateObject(const QObject &object);
            QSqlQuery deleteObject(const QObject &object);



        };
    }
}

#endif // SIMPLESQLMANAGER_H
