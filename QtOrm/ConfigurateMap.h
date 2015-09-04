#ifndef CONFIGURATEMAP_H
#define CONFIGURATEMAP_H


#include<QMap>
#include<QDebug>

#include <QMap>
#include <QString>

#include "ClassMap.h"
#include "ClassMapBase.h"
#include "configurate.h"

namespace QtOrm{
    namespace Config{
        using namespace QtOrm::Mapping;

        class ConfigurateMap{
        public:
            ConfigurateMap();
            template<class T>
            static void classRegister();
            static ClassMapBase* getMappedClass(QString className);
            static void writeRegisteredClassesToDebug();
        };

        template<class T>
        void ConfigurateMap::classRegister(){
            (void)static_cast<ClassMapBase*>((T*)0);

            T* classMap = new T();
            mappedClass.insert(classMap->getClassName(), classMap);
        }
    }
}

#endif // CONFIGURATEMAP_H
