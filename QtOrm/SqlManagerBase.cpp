#include "SqlManagerBase.h"
#include "ClassMapBase.h"

namespace QtOrm{
    namespace Config {
        extern QMap<QString, QtOrm::Mapping::ClassMapBase*> mappedClass;
    }

    namespace Sql{
        SqlManagerBase::SqlManagerBase(QSqlDatabase &database, QObject *parent)
            : QObject(parent), database(database){

        }

        QSqlQuery SqlManagerBase::getObjectById(const QString objectName, QVariant id){
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(objectName);
            this->resetTableNumber();

            QString tableAlias = this->generateTableAlias();
            QString select = getSelect(objectName);
            QString from = "";
            QString where = "";
            QSqlQuery *query = new QSqlQuery();

            //query.bindValue("", id);
            return *query;
        }

        QString QtOrm::Sql::SqlManagerBase::getSelect(const QString objectName) const{
            return "select *";
        }

        QString QtOrm::Sql::SqlManagerBase::getFrom(const QString objectName) const{

        }

        QString QtOrm::Sql::SqlManagerBase::getWhere(const QString objectName) const{

        }

        QString SqlManagerBase::generateTableAlias()
        {
            return QString("tb_%1").arg(++tableNumber);
        }

        void SqlManagerBase::resetTableNumber()
        {
            tableNumber = 0;
        }
    }
}
