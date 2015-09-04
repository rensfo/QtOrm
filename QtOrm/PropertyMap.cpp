#include "PropertyMap.h"

namespace QtOrm
{
    namespace Mapping
    {

        PropertyMap::PropertyMap()
        {

        }

        PropertyMap::PropertyMap(QString name) : name(name)
        {

        }

        PropertyMap::PropertyMap(QString name, QString column)  : name(name), column(column)
        {

        }

        PropertyMap &PropertyMap::setName(QString name)
        {
            this->name = name;
            return *this;
        }

        QString PropertyMap::getName()
        {
            return name;
        }

        PropertyMap &PropertyMap::setColumn(QString column)
        {
            this->column = column;
            return *this;
        }

        QString PropertyMap::getColumn()
        {
            return column;
        }

        PropertyMap &PropertyMap::setIsId(bool isId)
        {
            this->isId = isId;
            return *this;
        }

        bool PropertyMap::getIsId()
        {
            return isId;
        }

    }
}
