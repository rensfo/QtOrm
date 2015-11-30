#include "PostgreFunctionSqlBuilder.h"

namespace QtOrm {
namespace Sql {

PostgreFunctionSqlBuilder::PostgreFunctionSqlBuilder(const QSqlDatabase &database, QObject *parent)
    : SqlBuilderBase(database, parent) {
}

QSqlQuery PostgreFunctionSqlBuilder::insertObject(const QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  const QString paramName = "lNewId";
  QString declare = getDeclareText(*classBase, paramName);

  QString newRecord = getInsertFunctionText(*classBase, paramName);

  QString body;
  QString updateColumnText = getUpdateColumnText(*classBase);
  body = newRecord + updateColumnText;

  QString text = getAnonymousBlock(declare, body);
  QSqlQuery *query = new QSqlQuery(database);
  query->prepare(text);
  bindColumns(*classBase, *query, object);

  return *query;
}

QSqlQuery PostgreFunctionSqlBuilder::updateObject(const QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  QString updateText = getUpdateColumnText(*classBase);
  QString text = getAnonymousBlock("", updateText);
  QSqlQuery *query = new QSqlQuery(database);
  query->prepare(text);
  bindColumns(*classBase, *query, object);

  return *query;
}

QSqlQuery PostgreFunctionSqlBuilder::deleteObject(const QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  QString deleteText = getDeleteFunctionText(*classBase);
  QString text = getAnonymousBlock("", deleteText);
  QSqlQuery *query = new QSqlQuery(database);
  query->prepare(text);
  auto property = classBase->getIdProperty();
  query->bindValue(":" + property.getColumn(), object.property(property.getName().toStdString().data()));

  return *query;
}

QString PostgreFunctionSqlBuilder::getDeclareText(Mapping::ClassMapBase &classBase, QString paramName) {
  return QString("%1 %2.%3%type;\n")
      .arg(paramName)
      .arg(classBase.getTable())
      .arg(classBase.getIdProperty().getColumn());
}

QString PostgreFunctionSqlBuilder::getUpdateColumnText(Mapping::ClassMapBase &classBase) {
  QString psqlText = "";

  for (auto property : classBase.getProperties()) {
    if (property->getIsId())
      continue;

    QString columnUpdateText =
        getExecuteFunctionText(*property, classBase.getIdProperty().getColumn(), classBase.getContext());

    psqlText += columnUpdateText;
  }

  return psqlText;
}

QString PostgreFunctionSqlBuilder::getAnonymousBlock(QString declare, QString body) {
  return QString("do$$\n"
                 "declare\n"
                 "%1"
                 "begin\n"
                 "%2"
                 "end$$;")
      .arg(declare)
      .arg(body);
}

QString PostgreFunctionSqlBuilder::getExecuteFunctionText(const Mapping::PropertyMap &property,
                                                          const QString &idParamName, const QString &context) {
  QString functionText = getFunctionText(property.getUpdateFunction(), context);
  QString updateText = QString("perform %1(:%2, :%3);\n").arg(functionText).arg(idParamName).arg(property.getColumn());
  ;

  return updateText;
}

QString PostgreFunctionSqlBuilder::getInsertFunctionText(Mapping::ClassMapBase &classBase, QString paramName) {
  QString functionText = getFunctionText(classBase.getInsertFunction(), classBase.getContext());
  QString newRecord = QString("%1 := %2();\n").arg(paramName).arg(functionText);

  return newRecord;
}

QString PostgreFunctionSqlBuilder::getDeleteFunctionText(Mapping::ClassMapBase &classBase) {
  QString functionText = getFunctionText(classBase.getDeleteFunction(), classBase.getContext());
  QString deleteRecord = QString("perform %1(:%2);\n").arg(functionText).arg(classBase.getIdProperty().getColumn());

  return deleteRecord;
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

void PostgreFunctionSqlBuilder::bindColumns(Mapping::ClassMapBase &classBase, QSqlQuery &query, const QObject &object) {
  for (auto property : classBase.getProperties()) {
    if (property->getIsId())
      continue;

    query.bindValue(":" + property->getColumn(), object.property(property->getName().toStdString().data()));
  }
}
}
}
