#include "AutoUpdater.h"

#include <QDebug>
#include <QMetaObject>

#include "ConfigurationMap.h"
#include "Query.h"

namespace QtOrm {

using namespace Config;

AutoUpdater::AutoUpdater(QObject *parent) : QObject(parent) {
  onObjectPropertyChangedMethod = findOnObjectPropertyChangedMethod();
}

QString AutoUpdater::getPropertyName(QObject *sender, int senderSignalIndex) {
  QMetaMethod senderSignal = sender->metaObject()->method(senderSignalIndex);
  QMetaProperty senderProperty;
  const QMetaObject *senderMeta = sender->metaObject();
  for (int i = 0; i < senderMeta->propertyCount(); i++) {
    if (senderMeta->property(i).hasNotifySignal()) {
      if (senderMeta->property(i).notifySignal() == senderSignal) {
        senderProperty = senderMeta->property(i);
        break;
      }
    }
  }

  return QString(senderProperty.name());
}

void AutoUpdater::connectToAllProperties(QObject *object) {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass(object->metaObject()->className());
  QStringList properties;
  for (OneToOne *oneToOne : classBase->getOneToOneRelations())
    properties.append(oneToOne->getProperty());

  properties.append(classBase->getProperties().keys());

  for (QString propertyName : properties) {
    int propertyIndex = object->metaObject()->indexOfProperty(propertyName.toStdString().data());
    QMetaProperty property = object->metaObject()->property(propertyIndex);
    if (property.hasNotifySignal()) {
      connect(object, property.notifySignal(), this, onObjectPropertyChangedMethod);
    } else {
      if (classBase->getIdProperty().getName() != propertyName)
        qDebug() << QString("Property %1 from class %2 has not Notify signal!")
                        .arg(propertyName)
                        .arg(object->metaObject()->className());
    }
  }
}

void AutoUpdater::onObjectPropertyChanged() {
  QObject *sender = this->sender();
  if (sender) {
    QString senderPropertyName = getPropertyName(sender, senderSignalIndex());

    using namespace Sql;
    Query query;
    query.setDatabase(database);
    query.setReestr(reestr);

    connect(&query, &Query::executedSql, this, &AutoUpdater::executedSql);
    query.saveOneField(*sender, senderPropertyName);
  }
}

QMetaMethod AutoUpdater::findOnObjectPropertyChangedMethod() {
  QMetaMethod result;
  for (int m = 0; m < this->metaObject()->methodCount(); m++) {
    QMetaMethod method = this->metaObject()->method(m);
    if (QString(method.name()) == "onObjectPropertyChanged" && method.parameterCount() == 0) {
      result = method;
    }
  }

  return result;
}

QSqlDatabase AutoUpdater::getDatabase() const {
  return database;
}

void AutoUpdater::setDatabase(const QSqlDatabase &value) {
  database = value;
}

Reestr *AutoUpdater::getReestr() const {
  return reestr;
}

void AutoUpdater::setReestr(Reestr *value) {
  reestr = value;
}
}
