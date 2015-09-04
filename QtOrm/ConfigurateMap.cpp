#include "ConfigurateMap.h"

namespace QtOrm{
    namespace Config{
        using namespace QtOrm::Mapping;

        extern QMap<QString, ClassMapBase*> mappedClass;

        ConfigurateMap::ConfigurateMap(){
        }

        void ConfigurateMap::writeRegisteredClassesToDebug()
        {
            qDebug() << mappedClass;
        }

        const ClassMapBase* getMappedClass(QString className){
            return mappedClass.value(className);
        }
    }
}

