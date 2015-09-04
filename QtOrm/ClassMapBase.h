#ifndef CLASSMAPBASE_H
#define CLASSMAPBASE_H

#include <QString>
#include <QMap>
#include <QMetaObject>
#include <QDebug>

#include "PropertyMap.h"

namespace QtOrm{
    namespace Mapping{

        class ClassMapBase{
        public:
            ClassMapBase();

            QString getTable() const;
            void setTable(const QString &table);

            QString getClassName() const;

            const QMap<QString, PropertyMap *> getProperties() const;

            PropertyMap& id(QString propertyName);
            PropertyMap& map(QString propertyName);
            PropertyMap& map(QString propertyName, QString columnName);

            QMetaObject getMetaObject() const;
            void setMetaObject(const QMetaObject &metaObject);

        private:
            PropertyMap &createProperty(QString propertyName);

        private:
            QString table;
            QMap<QString, PropertyMap*> properties;

            QString idProperty; //??

            QMetaObject metaObject;
        };
    }
}

#endif // CLASSMAPBASE_H
