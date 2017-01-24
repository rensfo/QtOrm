#ifndef AUTOUPDATER_H
#define AUTOUPDATER_H

#include <QMetaProperty>
#include <QObject>
#include <QSqlDatabase>

#include "QueryCache.h"
#include "Registry.h"

namespace QtOrm {

class AutoUpdater : public QObject {
  Q_OBJECT
public:
  explicit AutoUpdater(QObject *parent = nullptr);

  QSharedPointer<Registry> getRegistry() const;
  void setRegistry(QSharedPointer<Registry> value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  QSharedPointer<QueryCache> getQueryCache() const;
  void setQueryCache(const QSharedPointer<QueryCache> &value);

protected:
  QString getPropertyName(QSharedPointer<QObject> sender, int senderSignalIndex);

signals:
  void executedSql(QString);

public slots:
  void connectToAllProperties(QSharedPointer<QObject> object);

protected slots:
  void onObjectPropertyChanged();

protected:
  QMetaMethod findOnObjectPropertyChangedMethod();

protected:
  QMetaMethod onObjectPropertyChangedMethod;
  QSharedPointer<Registry> registry;
  QSqlDatabase database;
  QSharedPointer<QueryCache> queryCache;
};
}

#endif // AUTOUPDATER_H
