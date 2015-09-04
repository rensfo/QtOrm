#include "ClassMapBase.h"
#include <QException>

namespace QtOrm{
    namespace Mapping{

        ClassMapBase::ClassMapBase(){

        }

        QString ClassMapBase::getTable() const{
            return table;
        }

        void ClassMapBase::setTable(const QString &table){
            this->table = table;
        }

        const QMap<QString, PropertyMap*> ClassMapBase::getProperties() const{
            return properties;
        }

        QString ClassMapBase::getClassName() const{
            return metaObject.className();
        }

        PropertyMap &ClassMapBase::id(QString propertyName){
            return createProperty(propertyName).setIsId(true);
        }

        PropertyMap &ClassMapBase::map(QString propertyName){
            return createProperty(propertyName);
        }

        PropertyMap &ClassMapBase::map(QString propertyName, QString columnName)
        {
            return createProperty(propertyName).setColumn(columnName);
        }

        PropertyMap &ClassMapBase::createProperty(QString propertyName){
            int propertyIndex = metaObject.indexOfProperty(propertyName.toStdString().data());
            if(propertyIndex == -1)
             qDebug() << QString("In class %1 Property %2 not found")
                            .arg(metaObject.className())
                            .arg(propertyName);


            PropertyMap *propertyMap = new PropertyMap(propertyName);
            properties.insert(propertyName, propertyMap);

            return *propertyMap;
        }

        QMetaObject ClassMapBase::getMetaObject() const{
            return metaObject;
        }

        void ClassMapBase::setMetaObject(const QMetaObject &metaObject){
            this->metaObject = metaObject;
        }

    }
}
