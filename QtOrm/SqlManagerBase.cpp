#include "SqlManagerBase.h"
#include "ClassMapBase.h"

namespace QtOrm{
    namespace Config {
        extern QMap<QString, QtOrm::Mapping::ClassMapBase*> mappedClass;
    }

    namespace Sql{
        SqlManagerBase::SqlManagerBase(const QSqlDatabase &database, QObject *parent)
             : QObject(parent), database(database) {
        }

        QString SqlManagerBase::generateTableAlias() {
            return QString("tb_%1").arg(++tableNumber);
        }

        void SqlManagerBase::resetTableNumber() {
            tableNumber = 0;
        }
    }
}
