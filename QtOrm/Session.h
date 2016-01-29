#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QVariant>
#include <QList>

#include "ConfigurateMap.h"
#include "Exception.h"
#include "SimpleSqlBuilder.h"

namespace QtOrm {
class Session : public QObject {
  Q_OBJECT
public:
  explicit Session(const QSqlDatabase &database, Sql::SqlBuilderType sqlManagerType, QObject *parent = 0);
  void insertObject(QObject &object);
  void updateObject(const QObject &object);
  void deleteObject(const QObject &object);
  template <class T> T *getById(const QVariant &id);
  template <class T> QList<T *> *getList();
  template <class T> QList<T *> *getList(const QString &property, const QVariant &value);
  template <class T> QList<QObject *> *getObjectList();
  template <class T> QList<QObject *> *getObjectList(const QString &property, const QVariant &value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &database);

  QTextStream *getTextStream() const;
  void setTextStream(QTextStream *value);

private:
  QSqlDatabase database;
  Sql::SqlBuilderBase *sqlBuilder;
  QTextStream *textStream;
};

template <class T> T *Session::getById(const QVariant &id) {
  QString className = T::staticMetaObject.className();
  return qobject_cast<T *>(sqlBuilder->getById(className, id));
}

template <class T> QList<T *> *Session::getList() {
//  QString className = T::staticMetaObject.className();
  return reinterpret_cast<QList<T *> *>(getObjectList<T>());
}

template <class T> QList<T *> *Session::getList(const QString &property, const QVariant &value) {
//  QString className = T::staticMetaObject.className();
//  QList<T *> *list = sqlBuilder->getListObject(className, property, value));
  return reinterpret_cast<QList<T *> *>(getObjectList<T>(property, value));
}

template <class T> QList<QObject *> *Session::getObjectList() {
  QString className = T::staticMetaObject.className();
  return sqlBuilder->getListObject(className);
}

template <class T> QList<QObject *> *Session::getObjectList(const QString &property, const QVariant &value) {
  QString className = T::staticMetaObject.className();
  return sqlBuilder->getListObject(className, property, value);
}
}
#endif // SESSION_H
