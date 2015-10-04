#include "SimpleSqlBuilder.h"

namespace QtOrm{
namespace Sql {
        SimpleSqlBuilder::SimpleSqlBuilder(const QSqlDatabase &database, QObject *parent)
            : SqlBuilderBase(database, parent){
        }

        QSqlQuery SimpleSqlBuilder::getListObject(const QString &objectName) {
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(objectName);
            resetTableNumber();

            this->generateTableAlias();
            QString select = getSelect();
            QString from = getFrom(classBase->getTable());

            QString fullSqlText = sqlQueryTemplate
                    .arg(select)
                    .arg(from)
                    .arg("");

            QSqlQuery *query = new QSqlQuery(database);
            query->prepare(fullSqlText);

            return *query;
        }

        QSqlQuery SimpleSqlBuilder::getListObject(const QString &objectName, const QString property, const QVariant value) {
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(objectName);
            resetTableNumber();
            QString tableAlias = this->generateTableAlias();
            QString select = getSelect();
            QString from = getFrom(classBase->getTable());
            QString column = classBase->getProperty(property).getColumn();
            QString placeHolder =  ":" + column;
            QString where = getWhere(tableAlias, column, placeHolder);

            QString fullSqlText = sqlQueryTemplate
                    .arg(select)
                    .arg(from)
                    .arg(where);

            QSqlQuery *query = new QSqlQuery(database);
            query->prepare(fullSqlText);
            query->bindValue(placeHolder, value);

            return *query;
        }

        QSqlQuery SimpleSqlBuilder::getObjectById(const QString &objectName, QVariant id) {
            Mapping::ClassMapBase* classBase = Config::ConfigurateMap::getMappedClass(objectName);
            resetTableNumber();

            QString tableAlias = this->generateTableAlias();
            QString select = getSelect();
            QString from = getFrom(classBase->getTable());
            QString idColumn = classBase->getIdProperty().getColumn();
            QString idPlaceHolder =  ":" + idColumn;
            QString where = getWhere(tableAlias, idColumn, idPlaceHolder);

            QString fullSqlText = sqlQueryTemplate
                    .arg(select)
                    .arg(from)
                    .arg(where);

            QSqlQuery *query = new QSqlQuery(database);
            query->prepare(fullSqlText);
            query->bindValue(idPlaceHolder, id);

            return *query;
        }

        QString SimpleSqlBuilder::getSelect() const {
            return QString("select %1.*").arg(getCurrentTableAlias());
        }

        QString SimpleSqlBuilder::getFrom(const QString &tableName) const {
            return QString("from %1 %2").arg(tableName).arg(getCurrentTableAlias());
        }

        QString SimpleSqlBuilder::getWhere(const QString &tableAlias, const QString &column, const QString &placeHolder) const {
            return QString("where %1.%2 = %3").arg(tableAlias).arg(column).arg(placeHolder);
        }

        QSqlQuery SimpleSqlBuilder::insertObject(const QObject &object) {
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

        QSqlQuery SimpleSqlBuilder::updateObject(const QObject &object) {
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

        QSqlQuery SimpleSqlBuilder::deleteObject(const QObject &object) {
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

