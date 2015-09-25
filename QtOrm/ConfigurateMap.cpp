#include "ConfigurateMap.h"

#include <QMap>
#include <QString>

namespace QtOrm{
    namespace Config{
        //using namespace QtOrm::Mapping;

        //extern QMap<QString, ClassMapBase*> mappedClass;

        ConfigurateMap::ConfigurateMap(){
        }

        ClassMapBase *ConfigurateMap::getMappedClass(QString className){
            return mappedClass.value(className);
        }

        void ConfigurateMap::writeRegisteredClassesToDebug(){
            qDebug() << mappedClass;
        }
    }
}

