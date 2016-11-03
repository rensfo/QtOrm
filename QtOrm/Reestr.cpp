#include "Reestr.h"

namespace QtOrm {

Reestr::Reestr(QObject *parent) : QObject(parent) {
}

bool Reestr::contains(const QString &table, const QString &id) {
  if (data.contains(table)) {
    return data.value(table).contains(id);
  }

  return false;
}

void Reestr::insert(const QString &table, const QString &id, QObject *object) {
  if (data.contains(table)) {
    if (!data.value(table).contains(id)) {
      data[table].insert(id, object);

      emit inserted(object);
    }
  } else {
    QHash<QString, QObject *> ids;
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

void Reestr::remove(QObject *object) {
  for (QHash<QString, QObject *> ids : data) {
    for (QObject *reestrObject : ids) {
      if (reestrObject == object) {
        QString key = ids.key(object);
        ids.remove(key);
        return;
      }
    }
  }
}

QObject *Reestr::value(const QString &table, const QString &id) {
  if (exists(table, id)) {
    return data[table][id];
  }

  return nullptr;
}

bool Reestr::exists(const QString &table, const QString &id) {
  if (data.contains(table))
    return data.value(table).contains(id);

  return false;
}
}
