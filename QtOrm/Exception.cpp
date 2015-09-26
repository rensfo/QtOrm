#include "Exception.h"

namespace QtOrm {
    Exception::Exception(const QString &message, QObject *parent)
        : QObject(parent), message(message){

    }

    Exception::Exception(const Exception &exception)
        : QObject(nullptr) {
        setParent(exception.parent());
        message = exception.getMessage();
    }

    QString Exception::getMessage() const {
        return message;
    }
}

