#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <QString>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMap>
#include <QDebug>

#include "ClassMapBase.h"

namespace QtOrm{
    namespace Mapping{

        template<class T>
        class ClassMap : public ClassMapBase{
        public:
            ClassMap();

        };

        template<class T>
        ClassMap<T>::ClassMap() : ClassMapBase()
        {
            (void)static_cast<QObject*>((T*)0);
            setMetaObject(T::staticMetaObject);
        }
    }
}

#endif // CLASSMAP_H
