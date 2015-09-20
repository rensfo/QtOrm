#ifndef SESSION_H
#define SESSION_H

#include <QObject>

namespace QtOrm {
    class Session : public QObject{
        Q_OBJECT
    public:
        explicit Session(QObject *parent = 0);
        void insertObject(const QObject &object);
        void updateObject(const QObject &object);
        void deleteObject(const QObject &object);
        template<T>
        T getById(QVariant id);

    signals:

    public slots:
    };
}
#endif // SESSION_H
