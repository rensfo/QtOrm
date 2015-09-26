#ifndef SIMPLESQLMANAGER_H
#define SIMPLESQLMANAGER_H

#include "SqlManagerBase.h"

namespace QtOrm{
    namespace Sql {
        class SimpleSqlManager : public SqlManagerBase
        {
            Q_OBJECT

        public:
            SimpleSqlManager(const QSqlDatabase &database, QObject *parent = 0);
            QSqlQuery getObjectById(const QString objectName, QVariant id);
            QString getSelect(const QString objectName) const;
            QString getFrom(const QString objectName) const;
            QString getWhere(const QString objectName) const ;
        };
    }
}

#endif // SIMPLESQLMANAGER_H
