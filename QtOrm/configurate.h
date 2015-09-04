#ifndef CONFIGURATE
#define CONFIGURATE

#include "ClassMap.h"

namespace QtOrm
{
    namespace Config
    {
        QMap<QString, QtOrm::Mapping::ClassMap*> mappedClass;
    }
}

#endif // CONFIGURATE

