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
  void saveObject(QSharedPointer<QObject> object);
  void deleteObject(QSharedPointer<QObject> object);
  template <class T>
  QSharedPointer<T> getById(const QVariant &id);
  template <class T>
  QSharedPointer<T> get(const Condition &filter);
  template <class T>
  QSharedPointer<T> get(const QString &property, const QVariant &value);
  template <class T>
  QList<QSharedPointer<T>> getList();
  template <class T>
  QList<QSharedPointer<T>> getList(const QString &property, const QVariant &value);
  template <class T>
  QList<QSharedPointer<T>> getList(const GroupConditions &conditions);
  template <class T>
  QList<QSharedPointer<QObject> > getObjectList();
  template <class T>
  QList<QSharedPointer<QObject> > getObjectList(const QString &property, const QVariant &value);
  template <class T>
  QList<QSharedPointer<QObject>> getObjectList(const GroupConditions &conditions);
  void refresh(QSharedPointer<QObject> value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &database);

  bool getAutoUpdate() const;
  void setAutoUpdate(bool value);

  void clearReestr();
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
  QSharedPointer<Reestr> reestr;
  AutoUpdater updater;
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
QSharedPointer<T> Session::get(const Condition &filter) {
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
  Condition c(property, Operation::Equal, value);
  return this->get<T>(c);
}

template <class T>
QList<QSharedPointer<T>> Session::getList() {
  return this->convertListTo<T>(getObjectList<T>());
}

template <class T>
QList<QSharedPointer<T>> Session::getList(const QString &property, const QVariant &value) {
  return convertListTo<T>(getObjectList<T>(property, value));
}

template <class T>
QList<QSharedPointer<T>> Session::getList(const GroupConditions &conditions) {
  return convertListTo<T>(getObjectList<T>(conditions));
}

template <class T>
QList<QSharedPointer<QObject>> Session::getObjectList() {
  QString className = T::staticMetaObject.className();
  Query query = createQuery();
  QList<QSharedPointer<QObject>> result = query.getListObject(className, QString(), QString(), QVariant());

  return result;
}

template <class T>
QList<QSharedPointer<QObject>> Session::getObjectList(const QString &property, const QVariant &value) {
  QString className = T::staticMetaObject.className();
  Query query = createQuery();
  QList<QSharedPointer<QObject>> result = query.getListObject(className, property, QString(), value);

  return result;
}

template <class T>
QList<QSharedPointer<QObject>> Session::getObjectList(const GroupConditions &conditions) {
  QString className = T::staticMetaObject.className();
  Query query = createQuery();
  QList<QSharedPointer<QObject>> result = query.getListObject(className, conditions);

  return result;
}

template <class T>
QList<QSharedPointer<T>> Session::convertListTo(QList<QSharedPointer<QObject>> list)
{
  QList<QSharedPointer<T>> convertedList;
  for(QSharedPointer<QObject> &item : list) {
    convertedList.append(item.objectCast<T>());
  }

  return convertedList;
}
}
#endif // SESSION_H
