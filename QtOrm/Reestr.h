#ifndef REESTR_H
#define REESTR_H

#include <QHash>
#include <QObject>

namespace QtOrm {

class Reestr : public QObject {
  Q_OBJECT
public:
  explicit Reestr(QObject *parent = nullptr);
  bool contains(const QString &table, const QString &id);
  void insert(const QString &table, const QString &id, QObject *object);
  void remove(const QString &table, const QString &id);
  void remove(QObject *object);
  QObject *value(const QString &table, const QString &id);

signals:
  void inserted(QObject *addedObject);

protected:
  bool exists(const QString &table, const QString &id);

private:
  QHash<QString, QHash<QString, QObject *>> data;
};
}
#endif // REESTR_H
