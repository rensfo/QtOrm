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

        QString SimpleSqlManager::getSelect(const QString objectName) const {

        }

        QString SimpleSqlManager::getFrom(const QString objectName) const {

        }

        QString SimpleSqlManager::getWhere(const QString objectName) const {

        }

        QSqlQuery SimpleSqlManager::insertObject(const QObject &object) {
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

            QString columns;
            QString values;
            foreach(auto prop, classBase->getProperties()){
                columns += (columns.isEmpty() ? "" : ", ") + prop->getColumn();
                values += (values.isEmpty() ? ":" : ", :") + prop->getColumn();
            }
            QString fullSqlText = QString("insert into %1(%2) values(%3)")
                    .arg(classBase->getTable())
                    .arg(columns)
                    .arg(values);

            QSqlQuery *query = new QSqlQuery(database);
            query->prepare(fullSqlText);
            foreach(auto prop, classBase->getProperties())
                query->bindValue(":" + prop->getColumn(), object.property(prop->getName().toStdString().c_str()));


            return *query;
        }

        QSqlQuery SimpleSqlManager::updateObject(const QObject &object) {
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

            QString setClause;
            QString whereClause;
            foreach(auto prop, classBase->getProperties()){
                if(prop->getIsId())
                    whereClause = QString("%1 = :%1").arg(prop->getColumn());
                else
                    setClause += QString("%1%2 = :%2")
                            .arg(setClause.isEmpty() ? "" : ", ")
                            .arg(prop->getColumn());
            }

            QString fullSqlText = QString("update %1 set %2 where %3")
                    .arg(classBase->getClassName())
                    .arg(setClause)
                    .arg(whereClause);
            QSqlQuery *query = new QSqlQuery(database);
            query->prepare(fullSqlText);
            foreach(auto prop, classBase->getProperties()){
                QString idPlaceHolder = ":" + prop->getColumn();
                QVariant idValue = object.property(prop->getName().toStdString().c_str());
                query->bindValue(idPlaceHolder, idValue);
            }

            return *query;
        }

        QSqlQuery SimpleSqlManager::deleteObject(const QObject &object) {
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

            QString idColumnName = classBase->getIdProperty().getColumn();
            QString idPlaceHolder = ":" + idColumnName;
            QString fullSqlText = QString("delete from %1 where %2 = %3")
                    .arg(classBase->getTable())
                    .arg(idColumnName)
                    .arg(idPlaceHolder);

            QSqlQuery *query = new QSqlQuery(database);
            query->prepare(fullSqlText);
            QVariant idPropertyValue = object.property(classBase->getIdProperty().getName().toStdString().c_str());
            query->bindValue(idPlaceHolder, idPropertyValue);

            return *query;
        }
    }
}

