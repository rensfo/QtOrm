#ifndef QUERY_H
#define QUERY_H

#include <QMap>
#include <QMetaProperty>
#include <QObject>
#include <QSignalMapper>
#include <QSqlDatabase>

#include "ClassMapBase.h"
#include "GroupConditions.h"
#include "OneToOne.h"
#include "QueryModel.h"
#include "Reestr.h"

namespace QtOrm {
namespace Sql {

using namespace Mapping;

class Query : public QObject {
  Q_OBJECT
public:
  explicit Query(QObject *parent = nullptr);
  Query(const Query &other);
  virtual QObject *getById(const QString &className, const QVariant &id);
  virtual QList<QObject *> *getListObject(const QString &className, const QString property = QString(),
                                          const QVariant value = QVariant());
  virtual QList<QObject *> *getListObject(const QString &className, const GroupConditions &conditions);
  virtual void saveObject(QObject &object);
  virtual void deleteObject(QObject &object);
  virtual void refresh(QObject &object);
  virtual void saveOneField(QObject &object, const QString &propertyName);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  Reestr *getReestr() const;
  void setReestr(Reestr *value);

  Query &operator=(const Query &other);

signals:
  void executedSql(QString sqlText);

protected:
  void insertObject(QObject &object);
  void updateObject(QObject &object);
  virtual void executeQuery(QSqlQuery &query);
  QList<QObject *> *getList(QSqlQuery &query, const QueryModel &queryModel);

  void fillObject(QObject &object, QueryTableModel *queryTableModel, const QSqlRecord &record);
  void fillOneToMany(const QList<OneToMany *> &relations, const QString &idProperty, QObject &object);
  void fillOneToOne(QObject &object, QueryTableModel *queryTableModel, const QSqlRecord &record);
  void objectSetProperty(QObject &object, const QString &propertyName, const QVariant &value);
  QObject *createNewInstance(Mapping::ClassMapBase &classBase);
  bool reestrContainsObject(Mapping::ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias);
  QObject *getObjectFromReestr(Mapping::ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias);
  void insertObjectIntoReestr(Mapping::ClassMapBase &classBase, const QSqlRecord &record, QObject *object,
                              const QString &tableAlias);
  void insertObjectIntoReestr(Mapping::ClassMapBase &classBase, QObject *object, QVariant idValue);
  void removeObjectFromReestr(QObject *object);
  QVariant getIdFromRecord(Mapping::ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias);

  void refreshObjectData(QObject &object, QueryTableModel *queryTableModel, const QSqlRecord &record);

  QString getSqlTextWithBindParams(QSqlQuery &query);

  void saveAllOneToOne(QObject &object);
  void saveOneToOne(QObject &object, OneToOne *oneToOne);

  void saveAllOneToMany(QObject &object);
  void saveOneToMany(QObject &object, OneToMany *oneToMany);

  void saveObjectWoStartTransaction(QObject &object);

  void startTransaction();
  void commit();
  void rollback();
  bool isIdObjectDefault(QObject &object);
  bool isIdOneToOneDefault(QObject &object, OneToOne *oneToOne);

protected:
  Reestr *reestr = nullptr;
  QSqlDatabase database;
};
}
}
#endif // QUERY_H
