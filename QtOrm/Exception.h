#ifndef EXEPTION_H
#define EXEPTION_H

#include <QObject>
#include <QString>

namespace QtOrm {
class Exception : public QObject {
  Q_OBJECT
public:
  explicit Exception(const QString &message, QObject *parent = 0);
  Exception(const Exception &exception);
  QString getMessage() const;
signals:

public slots:

private:
  QString message;
};
}

#endif // EXEPTION_H
