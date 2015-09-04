#include "ConfigurateMap.h"

namespace QtOrm
{
    namespace Config
    {
        using namespace QtOrm::Mapping;
        extern QMap<QString, ClassMap*> mappedClass;

        ConfigurateMap::ConfigurateMap()
        {

        }

        const ClassMap* getMappedClass(QString className)
        {
            return mappedClass.value(className);
        }
    }
}

