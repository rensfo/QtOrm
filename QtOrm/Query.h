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
#include "QueryModels/QueryModel.h"
#include "Registry.h"
#include "Relations/OneToOne.h"
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
  virtual QList<QSharedPointer<QObject>> getListObject(const QString &className, const QString &property = QString(),
                                                       const QVariant &value = QVariant(),
                                                       const QList<OrderColumn> &orderBy = QList<OrderColumn>());
  virtual QList<QSharedPointer<QObject>> getListObject(const QString &className, const GroupConditions &conditions,
                                                       const QList<OrderColumn> &orderBy = QList<OrderColumn>());
  virtual void saveObject(QSharedPointer<QObject> &object);
  virtual void deleteObject(QSharedPointer<QObject> &object);
  virtual void refresh(QSharedPointer<QObject> &object);
  virtual void saveOneField(QSharedPointer<QObject> &object, const QString &propertyName);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  QSharedPointer<Registry> getRegistry() const;
  void setRegistry(QSharedPointer<Registry> value);

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
  QList<QSharedPointer<QObject>> getList(QSqlQuery &query, const QSharedPointer<QueryModel> &queryModel);

  void fillObject(QSharedPointer<QObject> &object, QSharedPointer<QueryTableModel> &queryTableModel,
                  const QSqlRecord &record);
  void fillOneToMany(QSharedPointer<QObject> object, const QList<QSharedPointer<OneToMany>> &relations,
                     const QString &idProperty);
  void fillOneToOne(QSharedPointer<QObject> object, QSharedPointer<QueryTableModel> queryTableModel,
                    const QSqlRecord &record);
  QSharedPointer<QObject> getObject(const QSqlRecord &record, const QSharedPointer<ClassMapBase> &classBase,
                                    const QString &tableAlias, QSharedPointer<QueryTableModel> queryTableModel);
  void setObjectProperty(QSharedPointer<QObject> object, const QString &propertyName, QVariant &value);
  QSharedPointer<QObject> createNewInstance(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                            const QSharedPointer<QueryTableModel> &model);
  bool registryContainsObject(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                              const QString &tableAlias);
  QSharedPointer<QObject> getObjectFromRegistry(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                                const QString &tableAlias);
  void insertObjectIntoRegistry(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record,
                                QSharedPointer<QObject> object, const QString &tableAlias);
  void insertObjectIntoRegistry(QSharedPointer<ClassMapBase> classBase, QSharedPointer<QObject> object,
                                QVariant idValue);
  void removeObjectFromRegistry(QSharedPointer<QObject> object);
  QVariant getIdFromRecord(QSharedPointer<ClassMapBase> classBase, const QSqlRecord &record, const QString &tableAlias);

  void refreshObjectData(QSharedPointer<QObject> object, QSharedPointer<QueryTableModel> queryTableModel,
                         const QSqlRecord &record);

  QString getSqlTextWithBindParams(QSqlQuery &query);

  void saveAllOneToOne(QSharedPointer<QObject> object);
  void saveOneToOne(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne);

  void saveAllOneToMany(QSharedPointer<QObject> object);
  void saveOneToMany(QSharedPointer<QObject> object, QSharedPointer<OneToMany> oneToMany);

  void deleteAllOneToMany(QSharedPointer<QObject> object);
  void deleteOneToMany(QSharedPointer<QObject> object, const QSharedPointer<OneToMany> &oneToMany);

  void saveObjectWoStartTransaction(QSharedPointer<QObject> object);

  void startTransaction();
  void commit();
  void rollback();
  bool isIdObjectDefault(QSharedPointer<QObject> object);
  bool isIdOneToOneDefault(QSharedPointer<QObject> object, QSharedPointer<OneToOne> oneToOne);
  QString getQueryColumn(QSharedPointer<QueryTableModel> queryTableModel, QSharedPointer<PropertyMap> property);
  bool tryReopenDatabaseConnectionIfNeed();
  SimpleSqlBuilder createSimpleSqlBuilder(QSharedPointer<ClassMapBase> &classBase);
  GroupConditions replacePropertyToColumn(QSharedPointer<ClassMapBase> &classBase, const GroupConditions &conditions);
  QList<OrderColumn> replacePropertyToColumn(QSharedPointer<ClassMapBase> &classBase,
                                             const QList<OrderColumn> &orderColumns);
  QString getColumn(QSharedPointer<ClassMapBase> &classBase, const QString &property);
  void connectToAllProperties(QSharedPointer<QObject> &object);

protected:
  QSharedPointer<Registry> registry;
  QSqlDatabase database;
  QList<QSharedPointer<QObject>> refreshedObject;
  QSharedPointer<AutoUpdater> updater;
};
}
}
#endif // QUERY_H
