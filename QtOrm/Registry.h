#ifndef REGISTRY_H
#define REGISTRY_H

#include <QHash>
#include <QObject>
#include <QSharedPointer>

namespace QtOrm {

class Registry : public QObject {
  Q_OBJECT

  using RegistryData = QHash<QString, QSharedPointer<QObject>>;

public:
  explicit Registry(QObject *parent = nullptr);
  bool contains(const QString &table, const QString &id);
  void insert(const QString &table, const QString &id, QSharedPointer<QObject> object);
  void remove(const QString &table, const QString &id);
  void remove(QSharedPointer<QObject> object);
  QSharedPointer<QObject> value(const QString &table, const QString &id);
  QSharedPointer<QObject> value(QObject *object);

  void clear();

private:
  QHash<QString, RegistryData> data;
};
}
#endif // REGISTRY_H
