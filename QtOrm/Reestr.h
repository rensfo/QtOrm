#ifndef REESTR_H
#define REESTR_H

#include <QHash>
#include <QObject>
#include <QSharedPointer>

namespace QtOrm {

class Reestr : public QObject {
  Q_OBJECT

  using ReestrData = QHash<QString, QSharedPointer<QObject>>;

public:
  explicit Reestr(QObject *parent = nullptr);
  bool contains(const QString &table, const QString &id);
  void insert(const QString &table, const QString &id, QSharedPointer<QObject> object);
  void remove(const QString &table, const QString &id);
  void remove(QSharedPointer<QObject> object);
  QSharedPointer<QObject> value(const QString &table, const QString &id);
  QSharedPointer<QObject> value(QObject *object);

  void clear();

private:
  QHash<QString, ReestrData> data;
};
}
#endif // REESTR_H
