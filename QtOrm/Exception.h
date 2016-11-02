#ifndef EXEPTION_H
#define EXEPTION_H

#include <QObject>
#include <QString>

namespace QtOrm {

enum class ErrorGroup { Sql, MetaData };

class Exception : public QObject {
  Q_OBJECT
public:
  explicit Exception(ErrorGroup group, const QString &message,
                     QObject *parent = 0);
  Exception(const Exception &exception);
  QString getMessage() const;

  ErrorGroup getGroup() const;

private:
  ErrorGroup group;
  QString message;
};
}

#endif // EXEPTION_H
