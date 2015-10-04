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
            if(!idProperty.isEmpty()){
                QString errorText = QString("При связывании поля '%1' класса '%2' произошла исключительная ситуация - id уже зарезервировано за '%3'")
                        .arg(propertyName)
                        .arg(getClassName())
                        .arg(idProperty);
                throw new Exception(errorText);
            }

            idProperty = propertyName;
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
            checkToExistProperty(propertyName);

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

        PropertyMap &ClassMapBase::getIdProperty() {
            return *(properties.value(idProperty));
        }

        PropertyMap &ClassMapBase::getProperty(const QString &property){
            return *(properties.value(property));
        }

        OneToMany &ClassMapBase::oneToMany(const QString &property) {
            checkToExistProperty(property);

            OneToMany *relation = new OneToMany();
            oneToManyRelations.insert(property, relation);

            return *relation;
        }

        void ClassMapBase::checkToExistProperty(const QString &property) {
            int propertyIndex = metaObject.indexOfProperty(property.toStdString().data());
            if(propertyIndex == -1)
            {
                QString message = QString("В классе %1 свойство %2 не найдено.")
                        .arg(metaObject.className())
                        .arg(property);
                throw new Exception(message);
            }
        }

    }
}
