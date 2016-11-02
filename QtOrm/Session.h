#ifndef SESSION_H
#define SESSION_H

#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

#include "ConfigurationMap.h"
#include "Exception.h"
#include "Group.h"
#include "Query.h"
#include "SimpleSqlBuilder.h"

namespace QtOrm {

using namespace Sql;

class Session : public QObject {
  Q_OBJECT
public:
  explicit Session(QObject *parent = nullptr);
  void saveObject(QObject &object);
  void deleteObject(QObject &object);
  template <class T> T *getById(const QVariant &id);
  template <class T> T *get(const Filter &filter);
  template <class T> QList<T *> *getList();
  template <class T>
  QList<T *> *getList(const QString &property, const QVariant &value);
  template <class T> QList<T *> *getList(const Group &conditions);
  template <class T> QList<QObject *> *getObjectList();
  template <class T>
  QList<QObject *> *getObjectList(const QString &property,
                                  const QVariant &value);
  template <class T> QList<QObject *> *getObjectList(const Group &conditions);
  void refresh(QObject &value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &database);

  bool getAutoUpdate() const;
  void setAutoUpdate(bool value);

signals:
  void executeSql(QString sqlText);

private:
  QSqlDatabase database;
  Query query;
};

template <class T> T *Session::getById(const QVariant &id) {
  QString className = T::staticMetaObject.className();
  QObject *selectedObj = query.getById(className, id);

  if (!selectedObj)
    return nullptr;

  return qobject_cast<T *>(selectedObj);
}

template <class T> T *Session::get(const Filter &filter) {
  Group group;
  group.addFilter(filter);
  auto list = getList<T>(group);

  if (list->count() == 0) {
    return nullptr;
  }

  return list->first();
}

template <class T> QList<T *> *Session::getList() {
  return reinterpret_cast<QList<T *> *>(getObjectList<T>());
}

template <class T>
QList<T *> *Session::getList(const QString &property, const QVariant &value) {
  return reinterpret_cast<QList<T *> *>(getObjectList<T>(property, value));
}

template <class T> QList<T *> *Session::getList(const Group &conditions) {
  return reinterpret_cast<QList<T *> *>(getObjectList<T>(conditions));
}

template <class T> QList<QObject *> *Session::getObjectList() {
  QString className = T::staticMetaObject.className();
  QList<QObject *> *result;
  result = query.getListObject(className, QString(), QVariant());

  return result;
}

template <class T>
QList<QObject *> *Session::getObjectList(const QString &property,
                                         const QVariant &value) {
  QString className = T::staticMetaObject.className();
  QList<QObject *> *result;
  result = query.getListObject(className, property, value);

  return result;
}

template <class T>
QList<QObject *> *Session::getObjectList(const Group &conditions) {
  QString className = T::staticMetaObject.className();
  QList<QObject *> *result;
  result = query.getListObject(className, conditions);

  return result;
}
}
#endif // SESSION_H
