#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <QString>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMap>
#include <QDebug>

#include "PropertyMap.h"

namespace QtOrm
{
    namespace Mapping
    {
        class ClassMap
        {
        public:
            ClassMap(QString className);
            PropertyMap& id(QString property);
            PropertyMap& map(QString property);
            const QMap<QString, PropertyMap> getProperties() const;
            void setTable(const QString &table);
            const QString getTable() const;
            const QString getClassName() const;

        private:
            QString idProperty;
            QString className;
            QMap<QString, PropertyMap> properties;
            QString table;
        };
    }
}

#endif // CLASSMAP_H
