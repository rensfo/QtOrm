#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include <QString>

namespace QtOrm
{
    namespace Mapping
    {

        class PropertyMap
        {

        public:
            PropertyMap();
            PropertyMap(QString name);
            PropertyMap(QString name, QString column);
            PropertyMap& setName(QString name);
            QString getName();
            PropertyMap& setColumn(QString column);
            QString getColumn();
            PropertyMap& setIsId(bool isId);
            bool getIsId();

        private:
            QString name;
            QString column;
            bool isId;
        };
    }
}

#endif // PROPERTYMAP_H
