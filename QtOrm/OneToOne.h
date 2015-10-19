#ifndef ONETOONE_H
#define ONETOONE_H

#include <QString>

namespace QtOrm{
namespace Mapping{

class OneToOne{
public:
    OneToOne();
    QString getProperty() const;
    OneToOne &setProperty(const QString &property);

    QString getValueProperty() const;
    OneToOne &setValueProperty(const QString &column);

private:
    QString property;
    QString valueProperty;
};

}
}
#endif // ONETOONE_H
