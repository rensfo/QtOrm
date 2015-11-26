#include "SimpleSqlBuilder.h"

namespace QtOrm{
namespace Sql {
        SimpleSqlBuilder::SimpleSqlBuilder(const QSqlDatabase &database, QObject *parent)
            : SqlBuilderBase(database, parent){
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

