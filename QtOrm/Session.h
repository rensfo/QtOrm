#ifndef SESSION_H
#define SESSION_H

#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

#include "AutoUpdater.h"
#include "ConfigurationMap.h"
#include "Exception.h"
#include "GroupConditions.h"
#include "Query.h"
#include "SimpleSqlBuilder.h"

namespace QtOrm {

using namespace Sql;

class Session : public QObject {
  Q_OBJECT
public:
  explicit Session(QObject *parent = nullptr);
  template <class T>
  void saveObject(QSharedPointer<T> &object);
  template <class T>
  void deleteObject(QSharedPointer<T> &object);
  template <class T>
  QSharedPointer<T> getById(const QVariant &id);
  template <class T>
  QSharedPointer<T> get(const QSharedPointer<Condition> &filter);
  template <class T>
  QSharedPointer<T> get(const QString &property, const QVariant &value);
  template <class T>
  QSharedPointer<T> get(const QString &property, Operation operation, const QVariant &value);
  template <class T>
  QList<QSharedPointer<T>> getList();
  template <class T>
  QList<QSharedPointer<T>> getList(const QString &property, const QVariant &value);
  template <class T>
  QList<QSharedPointer<T>> getList(const QString &property, Operation operation, const QVariant &value);
  template <class T>
  QList<QSharedPointer<T>> getList(const GroupConditions &conditions);
  template <class T>
  QList<QSharedPointer<QObject>> getObjectList();
  template <class T>
  QList<QSharedPointer<QObject>> getObjectList(const QString &property, const QVariant &value);
  template <class T>
  QList<QSharedPointer<QObject>> getObjectList(const GroupConditions &conditions);
  template <class T>
  void refresh(QSharedPointer<T> &value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &database);

  bool getAutoUpdate() const;
  void setAutoUpdate(bool value);

  void clearRegistry();
  void clearQueryCache();

  void removeFromCache(QSharedPointer<QObject> object);

protected:
  Query createQuery();
  template <class T>
  QList<QSharedPointer<T>> convertListTo(QList<QSharedPointer<QObject>> list);

signals:
  void executedSql(QString sqlText);

protected:
  QSqlDatabase database;
  QSharedPointer<Registry> registry;
  QSharedPointer<AutoUpdater> updater;
  bool autoUpdate = false;
  QSharedPointer<QueryCache> queryCache;
};

template <class T>
QSharedPointer<T> Session::getById(const QVariant &id) {
  QString className = T::staticMetaObject.className();
  Query query = createQuery();
  QSharedPointer<QObject> selectedObj = query.getById(className, id);

  if (!selectedObj)
    return QSharedPointer<T>();

  return selectedObj.objectCast<T>();
}

template <class T>
QSharedPointer<T> Session::get(const QSharedPointer<Condition> &filter) {
  GroupConditions group;
  group.addCondition(filter);
  auto list = getList<T>(group);

  if (list.count() == 0) {
    return QSharedPointer<T>();
  }

  return list.first();
}

template <class T>
QSharedPointer<T> Session::get(const QString &property, const QVariant &value) {
  return get<T>(property, Operation::Equal, value);
}

template <class T>
QSharedPointer<T> Session::get(const QString &property, Operation operation, const QVariant &value) {
  QSharedPointer<Condition> c = QSharedPointer<Condition>::create(property, operation, value);

  return this->get<T>(c);
}

template <class T>
QList<QSharedPointer<T>> Session::getList() {
  return this->convertListTo<T>(getObjectList<T>());
}

template <class T>
QList<QSharedPointer<T>> Session::getList(const QString &property, const QVariant &value) {
  return getList<T>(property, Operation::Equal, value);
}

template <class T>
QList<QSharedPointer<T>> Session::getList(const QString &property, Operation operation, const QVariant &value) {
  return convertListTo<T>(getObjectList<T>(property, operation, value));
}

template <class T>
QList<QSharedPointer<T>> Session::getList(const GroupConditions &conditions) {
  return convertListTo<T>(getObjectList<T>(conditions));
}

template <class T>
QList<QSharedPointer<QObject>> Session::getObjectList() {
  return getObjectList<T>(QString(), QVariant());
}

template <class T>
QList<QSharedPointer<QObject>> Session::getObjectList(const QString &property, const QVariant &value) {
  GroupConditions conditions;
  conditions.addEqual(property, value);
  return getObjectList<T>(conditions);
}

template <class T>
QList<QSharedPointer<QObject>> Session::getObjectList(const GroupConditions &conditions) {
  QString className = T::staticMetaObject.className();
  Query query = createQuery();
  QList<QSharedPointer<QObject>> result = query.getListObject(className, conditions);

  return result;
}

template <class T>
QList<QSharedPointer<T>> Session::convertListTo(QList<QSharedPointer<QObject>> list) {
  QList<QSharedPointer<T>> convertedList;
  for (QSharedPointer<QObject> &item : list) {
    convertedList.append(item.objectCast<T>());
  }

  return convertedList;
}

template <class T>
void Session::saveObject(QSharedPointer<T> &object) {
  Query query = createQuery();
  QSharedPointer<QObject> qobject = object.template objectCast<QObject>();
  query.saveObject(qobject);
}

template <class T>
void Session::deleteObject(QSharedPointer<T> &object) {
  Query query = createQuery();
  QSharedPointer<QObject> qobject = object.template objectCast<QObject>();
  query.deleteObject(qobject);
}

template <class T>
void Session::refresh(QSharedPointer<T> &object) {
  Query query = createQuery();
  QSharedPointer<QObject> qobject = object.template objectCast<QObject>();
  query.refresh(qobject);
}
}
#endif // SESSION_H
