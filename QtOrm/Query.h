#ifndef QUERY_H
#define QUERY_H

#include <QMap>
#include <QMetaProperty>
#include <QObject>
#include <QPair>
#include <QSignalMapper>
#include <QSqlDatabase>

#include "ClassMapBase.h"
#include "OneToOne.h"
#include "SqlBuilderBase.h"

namespace QtOrm {

namespace Sql {

using namespace Mapping;

class Query : public QObject {
  Q_OBJECT
public:
  explicit Query(QObject *parent = nullptr);
  virtual QObject *getById(const QString &className, const QVariant &id);
  virtual QList<QObject *> *getListObject(const QString &className, const QString property = QString(),
                                          const QVariant value = QVariant());
  virtual QList<QObject *> *getListObject(const QString &className, const GroupConditions &conditions);
  virtual void saveObject(QObject &object);
  virtual void deleteObject(QObject &object);
  virtual void refresh(QObject &object);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  SqlBuilderBase *getSqlBuilder() const;
  void setSqlBuilder(SqlBuilderBase *value);

  QMap<OneToOne *, QString> getOneToOneAlias() const;

  bool getAutoUpdate() const;
  void setAutoUpdate(bool value);

signals:
  void executeSql(QString sqlText);

protected:
  void insertObject(QObject &object);
  void updateObject(QObject &object);
  virtual void executeQuery(QSqlQuery &query);
  QList<QObject *> *getList(Mapping::ClassMapBase &classBase, QSqlQuery &query, const QString &mainTableAlias);

  void fillObject(const Mapping::ClassMapBase &classBase, const QSqlRecord &record, const QString tableAlias,
                  QObject &object);
  void fillOneToMany(const QList<OneToMany *> &relations, const QString &idProperty, QObject &object);
  void fillOneToOne(QList<OneToOne *> &relations, QObject &object, const QSqlRecord &record);
  void objectSetProperty(QObject &object, const QString &propertyName, const QVariant &value);
  QObject *createNewInstance(Mapping::ClassMapBase &classBase);
  bool reestrContainsObject(Mapping::ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias);
  QObject *getObjectFromReestr(Mapping::ClassMapBase &classBase, const QSqlRecord &record, const QString &tableAlias);
  void insertObjectIntoReestr(Mapping::ClassMapBase &classBase, const QSqlRecord &record, QObject *object,
                              const QString &tableAlias);
  void insertObjectIntoReestr(Mapping::ClassMapBase &classBase, QObject *object, QVariant idValue);
  void removeObjectFromReestr(QObject *object);
  QPair<QString, QString> getReestrKey(Mapping::ClassMapBase &classBase, const QSqlRecord &record,
                                       const QString &tableAlias);

  void refreshObjectData(QObject &object, const QSqlRecord &record, const QString tableAlias);

  QString getSqlTextWithBindParams(QSqlQuery &query);

  void saveAllOneToOne(QObject &object);
  void saveOneToOne(QObject &object, OneToOne *oneToOne);

  void saveAllOneToMany(QObject &object);
  void saveOneToMany(QObject &object, OneToMany *oneToMany);

  void connectToProperties(QObject &object, const Mapping::ClassMapBase &classBase);
  QMetaMethod findOnObjectPropertyChangedMethod();
  QString getPropertyName(QObject *sender, int senderSignalIndex);

  void saveObjectWoStartTransaction(QObject &object);

  void startTransaction();
  void commit();
  void rollback();
  bool isIdObjectDefault(QObject &object);

protected slots:
  void onObjectPropertyChanged();

protected:
  QMap<QPair<QString, QString>, QObject *> reestr;
  QMap<OneToOne *, QString> oneToOneAlias;
  QSqlDatabase database;
  SqlBuilderBase *sqlBuilder = nullptr;
  bool autoUpdate = true;
  QMetaMethod onObjectPropertyChangedMethod;
};
}
}
#endif // QUERY_H
