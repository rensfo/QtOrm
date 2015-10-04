#include "SqlBuilderBase.h"
#include "ClassMapBase.h"

namespace QtOrm{
    namespace Config {
        extern QMap<QString, QtOrm::Mapping::ClassMapBase*> mappedClass;
    }

    namespace Sql{
        SqlBuilderBase::SqlBuilderBase(const QSqlDatabase &database, QObject *parent)
            : QObject(parent), database(database) {
        }

        QString SqlBuilderBase::generateTableAlias() {
            return tableAliasTemplate.arg(++tableNumber);
        }

        QString SqlBuilderBase::getCurrentTableAlias() const
        {
            return tableAliasTemplate.arg(tableNumber);
        }

        void SqlBuilderBase::resetTableNumber() {
            tableNumber = 0;
        }
    }
}
