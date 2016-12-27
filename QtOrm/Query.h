#ifndef QUERY_H
#define QUERY_H

#include <QMap>
#include <QMetaProperty>
#include <QObject>
#include <QSharedPointer>
#include <QSignalMapper>
#include <QSqlDatabase>

#include "AutoUpdater.h"
#include "ClassMapBase.h"
#include "GroupConditions.h"
#include "OneToOne.h"
#include "QueryCache.h"
#include "QueryModel.h"
#include "Reestr.h"
#include "SimpleSqlBuilder.h"

namespace QtOrm {
namespace Sql {

using namespace Mapping;

class Query : public QObject {
  Q_OBJECT
public:
  explicit Query(QObject *parent = nullptr);
  Query(const Query &other);
  virtual QSharedPointer<QObject> getById(const QString &className, const QVariant &id);
  virtual QList<QSharedPointer<QObject> > getListObject(const QString &className, const QString &property = QString(), const QString &column = QString(),
                                          const QVariant &value = QVariant());
  virtual QList<QSharedPointer<QObject> > getListObject(const QString &className, const GroupConditions &conditions);
  virtual void saveObject(QSharedPointer<QObject> &object);
  virtual void deleteObject(QSharedPointer<QObject> &object);
  virtual void refresh(QSharedPointer<QObject> &object);
  virtual void saveOneField(QSharedPointer<QObject> &object, const QString &propertyName);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  QSharedPointer<Reestr> getReestr() const;
  void setReestr(QSharedPointer<Reestr> value);

  QSharedPointer<QueryCache> getQueryCache() const;
  void setQueryCache(QSharedPointer<QueryCache> value);

  void clearRefreshedObject();

  QSharedPointer<QtOrm::AutoUpdater> getUpdater() const;
  void setUpdater(const QSharedPointer<AutoUpdater> &value);

  Query &operator=(const Query &other);

signals:
  void executedSql(QString sqlText);

protected:
  void insertObject(QSharedPointer<QObject> &object);
  void updateObject(QSharedPointer<QObject> &object);
  virtual void executeQuery(QSqlQuery &query);
  QList<QSharedPointer<QObject> > getList(QSqlQuery &query, const QueryModel &queryModel);

  void fillObject(QSharedPointer<QObject> &object, QSharedPointer<QueryTableModel> &queryTableModel, const QSqlRecord &record);
  void fillOneToMany(const QList<QSharedPointer<OneToMany>> &relations, const QString &idProperty, QSharedPointer<QObject> object);
  void fillOneToOne(QSharedPointer<QObject> object, QSharedPointer<QueryTableModel> queryTableModel, const QSqlRecord &record);
  QSharedPointer<QObject> getObject(const QSqlRecord &record, const QSharedPointer<ClassMapBase> &classBase, const QString &tableAlias, QSharedPointer<QueryTableModel> queryTableModel);
  void objectSetProperty(QSharedPointer<QObject> object, const QString &propertyName, const QVariant &value);
  QSharedPointer<QObject> createNewInstance(QSharedPointer<ClassMapBase> classBase);
  bool reestrContainsObject(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, const QString &tableAlias);
  QSharedPointer<QObject> getObjectFromReestr(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, const QString &tableAlias);
  void insertObjectIntoReestr(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, QSharedPointer<QObject> object,
                              const QString &tableAlias);
  void insertObjectIntoReestr(QSharedPointer<ClassMapBase> classBase, QSharedPointer<QObject> object, QVariant idValue);
  void removeObjectFromReestr(QSharedPointer<QObject> object);
  QVariant getIdFromRecord(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, const QString &tableAlias);

  void refreshObjectData(QSharedPointer<QObject> object, QSharedPointer<QueryTableModel> queryTableModel, const QSqlRecord &record);

  QString getSqlTextWithBindParams(QSqlQuery &query);

  void saveAllOneToOne(QSharedPointer<QObject> object);
  void saveOneToOne(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne);

  void saveAllOneToMany(QSharedPointer<QObject> object);
  void saveOneToMany(QSharedPointer<QObject> object, QSharedPointer<OneToMany> oneToMany);

  void saveObjectWoStartTransaction(QSharedPointer<QObject> object);

  void startTransaction();
  void commit();
  void rollback();
  bool isIdObjectDefault(QSharedPointer<QObject> object);
  bool isIdOneToOneDefault(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne);
  QString getQueryColumn(QSharedPointer<QueryTableModel> queryTableModel, QSharedPointer<PropertyMap> property);
  bool tryReopenDatabaseConnectionIfNeed();
  SimpleSqlBuilder createSimpleSqlBuilder(QSharedPointer<ClassMapBase> &classBase);

protected:
  QSharedPointer<Reestr> reestr;
  QSqlDatabase database;
  QSharedPointer<QueryCache> queryCache;
  QList<QSharedPointer<QObject>> refreshedObject;
  QSharedPointer<AutoUpdater> updater;
};
}
}
#endif // QUERY_H
