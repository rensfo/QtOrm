#include "SimpleSqlManager.h"

namespace QtOrm{
    namespace Sql {
        SimpleSqlManager::SimpleSqlManager(const QSqlDatabase &database, QObject *parent)
            : SqlManagerBase(database, parent){
        }

        QSqlQuery SimpleSqlManager::getObjectById(const QString objectName, QVariant id) {
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(objectName);
            resetTableNumber();

            QString tableAlias = this->generateTableAlias();
            QString select = QString("select %1.*").arg(tableAlias);
            QString from = QString("from %1 %2").arg(classBase->getTable()).arg(tableAlias);
            QString idColumn = classBase->getIdProperty().getColumn();
            QString idPlaceHolder =  ":" + idColumn;
            QString where = QString("where %1.%2 = %3")
                    .arg(tableAlias)
                    .arg(idColumn)
                    .arg(idPlaceHolder);

            QString fullSqlText = sqlQueryTemplate
                    .arg(select)
                    .arg(from)
                    .arg(where);

            QSqlQuery *query = new QSqlQuery(database);
            query->prepare(fullSqlText);
            query->bindValue(idPlaceHolder, id);

            return *query;
        }

        QString SimpleSqlManager::getSelect(const QString objectName) const
        {

        }

        QString SimpleSqlManager::getFrom(const QString objectName) const
        {

        }

        QString SimpleSqlManager::getWhere(const QString objectName) const
        {

        }
    }
}

