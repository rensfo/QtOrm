#ifndef EXEPTION_H
#define EXEPTION_H

#include <QException>
#include <QString>

namespace QtOrm {

enum class ErrorCode {
  FindMoreThatOneRecord,
  Sql,
  NotRegistredClass,
  IdPropertyAlreadyRegistred,
  PropertyNotFound,
  UnableToSetValue,
  InstanceNotCreated
};

class Exception : public QException {
public:
  explicit Exception(ErrorCode code, const QString &message);
  Exception(const Exception &exception);

  virtual void raise() const override;
  virtual QException *clone() const override;

  QString getMessage() const;

  ErrorCode getCode() const;

protected:
  ErrorCode code;
  QString message;
};
}

#endif // EXEPTION_H
