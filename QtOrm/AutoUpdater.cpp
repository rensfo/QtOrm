#include "AutoUpdater.h"

#include <QDebug>
#include <QMetaObject>

#include "Mappings/ConfigurationMap.h"
#include "Query.h"

namespace QtOrm {

using namespace Config;

AutoUpdater::AutoUpdater(QObject *parent) : QObject(parent) {
  onObjectPropertyChangedMethod = findOnObjectPropertyChangedMethod();
}

QString AutoUpdater::getPropertyName(QSharedPointer<QObject> sender, int senderSignalIndex) {
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

void AutoUpdater::connectToAllProperties(QSharedPointer<QObject> object) {
  QSharedPointer<ClassMapBase> classBase = configuration->getMappedClass(object->metaObject()->className());
  QStringList properties;
  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()) {
    properties.append(oneToOne->getProperty());
  }

  if(SubClassMap::isClassTableInheritance(classBase)){
    auto allProperties = classBase->toSubclass()->getAllProperties();
    properties.append(allProperties.keys());
  } else {
    properties.append(classBase->getProperties().keys());
  }

  for (QString propertyName : properties) {
    int propertyIndex = object->metaObject()->indexOfProperty(propertyName.toStdString().data());
    QMetaProperty property = object->metaObject()->property(propertyIndex);
    if (property.hasNotifySignal()) {
      connect(object.data(), property.notifySignal(), this, onObjectPropertyChangedMethod, Qt::UniqueConnection);
    } else {
      if (classBase->getIdProperty()->getName() != propertyName)
        qWarning() << QString("Property %1 from class %2 has not Notify signal!")
                          .arg(propertyName)
                          .arg(object->metaObject()->className());
    }
  }
}

void AutoUpdater::onObjectPropertyChanged() {
  QObject *sender = this->sender();
  QSharedPointer<QObject> sharedSender = registry->value(sender);
  if (sharedSender) {
    QString senderPropertyName = getPropertyName(sharedSender, senderSignalIndex());

    using Sql::Query;
    Query query;
    query.setDatabase(database);
    query.setRegistry(registry);
    query.setConfiguration(configuration);

    connect(&query, &Query::executedSql, this, &AutoUpdater::executedSql);
    query.saveOneField(sharedSender, senderPropertyName);
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

QSharedPointer<Config::ConfigurationMap> AutoUpdater::getConfiguration() const
{
  return configuration;
}

void AutoUpdater::setConfiguration(QSharedPointer<ConfigurationMap> value)
{
  configuration = value;
}

QSqlDatabase AutoUpdater::getDatabase() const {
  return database;
}

void AutoUpdater::setDatabase(const QSqlDatabase &value) {
  database = value;
}

QSharedPointer<Registry> AutoUpdater::getRegistry() const {
  return registry;
}

void AutoUpdater::setRegistry(QSharedPointer<Registry> value) {
  registry = value;
}
}
