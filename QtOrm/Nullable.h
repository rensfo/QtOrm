#ifndef NULLABLE_H
#define NULLABLE_H

#include "NullableBase.h"
#include <QMetaType>

template <class T> class Nullable : public NullableBase {
public:
  Nullable();
  Nullable(const Nullable<T> &value);
  Nullable(T &value);

  T getValue() const;
  void setValue(const T &value);

  virtual bool hasValue() const;

  Nullable<T> &operator=(const T &value);
  Nullable<T> &operator=(const Nullable<T> &value);

private:
  T value;
  bool isNull;
};

template <class T> Nullable<T>::Nullable() : NullableBase() ,isNull(true) {
}

template <class T> Nullable<T>::Nullable(const Nullable<T> &value) : Nullable<T>(){
    isNull = !hasValue();
    this->value = value.getValue();
}

template <class T> Nullable<T>::Nullable(T &value) : Nullable<T>() {
  this->value = value;
}

template <class T> T Nullable<T>::getValue() const {
  return value;
}

template <class T> void Nullable<T>::setValue(const T &value) {
  value = value;
  isNull = false;
}

template <class T> bool Nullable<T>::hasValue() const {
  return !isNull;
}

template <class T> Nullable<T> &Nullable<T>::operator=(const T &value) {
  this->value = value;
  isNull = false;
  return *this;
}

template <class T> Nullable<T> &Nullable<T>::operator=(const Nullable<T> &value) {
  this->value = value.getValue();
  isNull = !value.hasValue();
  return *this;
}

#define ORM_DECLARE_METATYPE_TEMPLATE_1ARG(SINGLE_ARG_TEMPLATE) \
QT_BEGIN_NAMESPACE \
template <typename T> \
struct QMetaTypeId< SINGLE_ARG_TEMPLATE<T> > \
{ \
    enum { \
        Defined = QMetaTypeId2<T>::Defined \
    }; \
    static int qt_metatype_id() \
    { \
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
        if (const int id = metatype_id.load()) \
            return id; \
        const char *tName = QMetaType::typeName(qMetaTypeId<T>()); \
        Q_ASSERT(tName); \
        const int tNameLen = int(qstrlen(tName)); \
        QByteArray typeName; \
        typeName.reserve(int(sizeof(#SINGLE_ARG_TEMPLATE)) + 1 + tNameLen + 1 + 1); \
        typeName.append(#SINGLE_ARG_TEMPLATE, int(sizeof(#SINGLE_ARG_TEMPLATE)) - 1) \
            .append('<').append(tName, tNameLen); \
        if (typeName.endsWith('>')) \
            typeName.append(' '); \
        typeName.append('>'); \
        const int newId = qRegisterMetaType< SINGLE_ARG_TEMPLATE<T> >(typeName, reinterpret_cast< SINGLE_ARG_TEMPLATE<T> *>(quintptr(-1))); \
        metatype_id.storeRelease(newId); \
        return newId; \
    } \
}; \

ORM_DECLARE_METATYPE_TEMPLATE_1ARG(Nullable)

#endif // NULLABLE_H
