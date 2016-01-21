#include "PostgreFunctionSqlBuilder.h"
#include <QSqlError>

namespace QtOrm {
namespace Sql {

PostgreFunctionSqlBuilder::PostgreFunctionSqlBuilder(const QSqlDatabase &database, QObject *parent)
    : SqlBuilderBase(database, parent) {
}

void PostgreFunctionSqlBuilder::insertObject(QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  auto idValue = insertRecord(*classBase);
  object.setProperty(classBase->getIdProperty().getName().toStdString().c_str(), idValue);
  updateRecord(object, *classBase);
}

void PostgreFunctionSqlBuilder::updateObject(const QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  updateRecord(object, *classBase);
}

void PostgreFunctionSqlBuilder::deleteObject(const QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  deleteRecord(object, *classBase);
}

QMap<Config::PropertyMap *, QString> *PostgreFunctionSqlBuilder::getUpdateColumnText(Mapping::ClassMapBase &classBase) {
  QMap<Config::PropertyMap *, QString> *updates = new QMap<Config::PropertyMap *, QString>();

  for (auto property : classBase.getProperties()) {
    if (property->getIsId())
      continue;

    QString columnUpdateText =
        getUpdateFunctionText(*property, classBase.getIdProperty().getColumn(), classBase.getContext());

    updates->insert(property, columnUpdateText);
  }

  return updates;
}

QString PostgreFunctionSqlBuilder::getUpdateFunctionText(const Mapping::PropertyMap &property,
                                                         const QString &idParamName,
                                                         const QString &context) {
  QString functionText = getFunctionText(property.getUpdateFunction(), context);
  QString updateText = QString("%1(:%2, :%3)").arg(functionText).arg(idParamName).arg(property.getColumn());

  return wrapSelect(updateText);
}

QString PostgreFunctionSqlBuilder::getInsertFunctionText(Mapping::ClassMapBase &classBase) {
  QString functionText = getFunctionText(classBase.getInsertFunction(), classBase.getContext());
  QString newRecord = QString("%1()").arg(functionText);

  return wrapSelect(newRecord);
}

QString PostgreFunctionSqlBuilder::getDeleteFunctionText(Mapping::ClassMapBase &classBase) {
  QString functionText = getFunctionText(classBase.getDeleteFunction(), classBase.getContext());
  QString deleteRecord = QString("%1(:%2)").arg(functionText).arg(classBase.getIdProperty().getColumn());

  return wrapSelect(deleteRecord);
}

QString PostgreFunctionSqlBuilder::getFunctionText(const QString &functionName, const QString &context) {
  QString text;
  if (context.isEmpty()) {
    text = functionName;
  } else {
    text = QString("%1.%2").arg(context).arg(functionName);
  }

  return text;
}

QString PostgreFunctionSqlBuilder::wrapSelect(const QString &functionText) {
  return QString("select %1;").arg(functionText);
}

QVariant PostgreFunctionSqlBuilder::insertRecord(Mapping::ClassMapBase &classBase) {
  QSqlQuery query(database);

  QString newRecord = getInsertFunctionText(classBase);
  if (!query.exec(newRecord))
    throw new Exception(query.lastError().text());

  query.next();
  QVariant newId = query.value(0);

  return newId;
}

void PostgreFunctionSqlBuilder::updateRecord(const QObject &object, Mapping::ClassMapBase &classBase) {
  auto updateText = getUpdateColumnText(classBase);
  auto idValue = object.property(classBase.getIdProperty().getName().toStdString().c_str());
  QSqlQuery query(database);
  for (auto it = updateText->begin(); it != updateText->end(); ++it) {
    query.clear();
    query.prepare(it.value());
    query.bindValue(getPlaceHolder(classBase.getIdProperty().getName()), idValue);
    query.bindValue(getPlaceHolder(it.key()->getColumn()), object.property(it.key()->getName().toStdString().c_str()));
    if (!query.exec())
      throw new Exception(query.lastError().text());
  }
}

void PostgreFunctionSqlBuilder::deleteRecord(const QObject &object, Mapping::ClassMapBase &classBase) {
  QString deleteText = getDeleteFunctionText(classBase);
  QSqlQuery query(database);
  query.prepare(deleteText);
  auto property = classBase.getIdProperty();
  query.bindValue(getPlaceHolder(property.getColumn()), object.property(property.getName().toStdString().c_str()));
}
}
}
