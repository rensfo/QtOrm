#ifndef CONFIGURATEMAP_H
#define CONFIGURATEMAP_H


#include<QMap>
#include<QDebug>

#include "ClassMap.h"

namespace QtOrm
{
    namespace Config
    {
        using namespace QtOrm::Mapping;

        extern QMap<QString, ClassMap*> mappedClass;

        class ConfigurateMap
        {
        public:
            ConfigurateMap();
            template<class T>
            static void classRegister();
            static ClassMap* getMappedClass(QString className);
        };

        template<class T>
        void ConfigurateMap::classRegister()
        {
            T classMap;
            mappedClass.insert(classMap.getClassName(), &classMap);
        }



    }
}

#endif // CONFIGURATEMAP_H
