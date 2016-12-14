#include "Reestr.h"

#include <QMetaObject>

namespace QtOrm {

Reestr::Reestr(QObject *parent) : QObject(parent) {
}

bool Reestr::contains(const QString &table, const QString &id) {
  if (data.contains(table)) {
    return data.value(table).contains(id);
  }

  return false;
}

void Reestr::insert(const QString &table, const QString &id, QSharedPointer<QObject> object) {
  if (data.contains(table)) {
    if (!data.value(table).contains(id)) {
      data[table].insert(id, object);

      emit inserted(object);
    }
  } else {
    ReestrData ids;
    ids.insert(id, object);
    data.insert(table, ids);

    emit inserted(object);
  }
}

void Reestr::remove(const QString &table, const QString &id) {
  if (exists(table, id)) {
    data[table].remove(id);
  }
}

void Reestr::remove(QSharedPointer<QObject> object) {
  for (ReestrData &ids : data) {
    for (QSharedPointer<QObject> &reestrObject : ids) {
      if (reestrObject == object) {
        QString key = ids.key(object);
        ids.remove(key);
        return;
      }
    }
  }
}

QSharedPointer<QObject> Reestr::value(const QString &table, const QString &id) {
  if (exists(table, id)) {
    return data[table][id];
  }

  return QSharedPointer<QObject>();
}

QSharedPointer<QObject> Reestr::value(QObject *object) {
  QString className = object->metaObject()->className();
  if(data.contains(className)){
    for(QSharedPointer<QObject> &reestrObject : data[className]){
      if(reestrObject.data() == object){
        return reestrObject;
      }
    }
  }

  return QSharedPointer<QObject>();
}

void Reestr::clear() {
  data.clear();
}

bool Reestr::exists(const QString &table, const QString &id) {
  if (data.contains(table))
    return data.value(table).contains(id);

  return false;
}
}
