#ifndef CLASSMAPBASE_H
#define CLASSMAPBASE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QMetaObject>
#include <QDebug>

#include "PropertyMap.h"
#include "Exception.h"
#include "OneToMany.h"
#include "OneToOne.h"

namespace QtOrm{
    namespace Mapping{

        class ClassMapBase : public QObject{
        public:
            explicit ClassMapBase(QObject *parent = 0);

            QString getTable() const;
            void setTable(const QString &table);

            QString getClassName() const;

            const QMap<QString, PropertyMap *> getProperties() const;

            PropertyMap& id(QString propertyName);
            PropertyMap& map(QString propertyName);
            PropertyMap& map(QString propertyName, QString columnName);

            QMetaObject getMetaObject() const;
            void setMetaObject(const QMetaObject &metaObject);

            PropertyMap& getIdProperty();
            PropertyMap& getProperty(const QString &property);

            OneToMany &oneToMany(const QString &property);
            OneToOne &oneToOne(const QString &property);

            QMap<QString, OneToMany *> getOneToManyRelations() const;
            QMap<QString, OneToOne *> getOneToOneRelations() const;

        private:
            PropertyMap &createProperty(QString propertyName);
            void checkToExistProperty(const QString &property);

        private:
            QString table;
            QMap<QString, PropertyMap*> properties;
            QMap<QString, OneToMany*> oneToManyRelations;
            QMap<QString, OneToOne*> oneToOneRelations;

            QString idProperty;

            QMetaObject metaObject;
        };
    }
}

#endif // CLASSMAPBASE_H