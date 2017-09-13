#ifndef EXEPTION_H
#define EXEPTION_H

#include <QException>
#include <QString>

namespace QtOrm {

class Exception : public QException {
public:
  explicit Exception(const QString &message);
  Exception(const Exception &exception);

  virtual void raise() const override;
  virtual QException *clone() const override;

  QString getMessage() const;
  virtual const char*what() const noexcept override;

protected:
  QString message;
};

class FindMoreThatOneRecordException : public Exception {
public:
  using Exception::Exception;
};

class NotRegistredClassException : public Exception {
public:
  using Exception::Exception;
};

class IdPropertyAlreadyRegistredException : public Exception {
public:
  using Exception::Exception;
};

class DiscriminatorPropertyAlreadyRegistredException : public Exception {
public:
  using Exception::Exception;
};

class PropertyNotFoundException : public Exception {
public:
  using Exception::Exception;
};

class UnableToSetValueException : public Exception {
public:
  using Exception::Exception;
};

class InstanceNotCreatedException : public Exception {
public:
  using Exception::Exception;
};

class SqlException : public Exception {
public:
  using Exception::Exception;
};

class DatabaseConnectionClosedException : public Exception {
public:
  using Exception::Exception;
};
}

#endif // EXEPTION_H
