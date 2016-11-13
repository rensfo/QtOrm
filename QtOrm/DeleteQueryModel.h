#ifndef DELETEQUERYMODEL_H
#define DELETEQUERYMODEL_H

#include <QObject>

class DeleteQueryModel : public QObject
{
  Q_OBJECT
public:
  explicit DeleteQueryModel(QObject *parent = 0);

signals:

public slots:
};

#endif // DELETEQUERYMODEL_H