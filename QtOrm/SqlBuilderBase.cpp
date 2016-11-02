#include "SqlBuilderBase.h"

#include <QMetaProperty>
#include <QSqlError>
#include <QSqlRecord>
#include <QString>

#include "ClassMapBase.h"
#include "ConfigurationMap.h"

namespace QtOrm {
namespace Sql {
using namespace Config;

SqlBuilderBase::SqlBuilderBase(QObject *parent)
    : QObject(parent), tableNumber(0) {}

QSqlQuery SqlBuilderBase::selectQuery() {
  fillOneToOneAlias();
  QString select = getSelect(*classBase);
  QString from = getFrom(*classBase);

  QString where;
  if (!conditions.isEmpty()) {
    where = getWhere(tableAlias, conditions);
    if (!where.isEmpty())
      where = QString("where %1").arg(where);
  }

  QString fullSqlText = sqlQueryTemplate.arg(select).arg(from).arg(where);

  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindValues(query, conditions);

  return query;
}

QSqlQuery SqlBuilderBase::selectQuery(const QString &queryText) {
  QString where;
  if (!conditions.isEmpty()) {
    where = getWhere(tableAlias, conditions);
    if (!where.isEmpty())
      where = QString(" where %1").arg(where);
  }

  QString fullSqlText = queryText + where;

  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindValues(query, conditions);

  return query;
}

QString SqlBuilderBase::getPlaceHolder(const QString param) {
  return QString(":%1").arg(param);
}

QVariant SqlBuilderBase::prepareValue(QVariant &value) {
  //if (value.canConvert<NullableBase>())
  //  return value.value<NullableBase>().getVariant();

  return value;
}

QString SqlBuilderBase::getLikeCondition(const QString &tableName,
                                         const QString &fieldName) const {
  return QString("%1.%2 like '%' || :%2 || '%'").arg(tableName).arg(fieldName);
}

QString SqlBuilderBase::getSelect(const Mapping::ClassMapBase &classBase) {
  QString result = "";

  for (PropertyMap *propertyMap : classBase.getProperties()) {
    if (result.isEmpty())
      result = QString("select %1.%2 as %1_%2")
                   .arg(tableAlias)
                   .arg(propertyMap->getColumn());
    else
      result +=
          QString(", %1.%2 as %1_%2").arg(tableAlias, propertyMap->getColumn());
  }

  for (OneToOne *oneToOne : classBase.getOneToOneRelations())
    result += getOneToOneSelect(classBase, oneToOne);

  return result;
}

QString
SqlBuilderBase::getOneToOneSelect(const Mapping::ClassMapBase &classBase,
                                  OneToOne *oneToOne) {
  QString result = "";

  QString tableAlias = oneToOneAlias.value(oneToOne);

  QString property = oneToOne->getProperty();
  QString refClass = getTypeNameOfProperty(classBase.getMetaObject(), property);
  Mapping::ClassMapBase *refClassBase =
      ConfigurationMap::getMappedClass(refClass);
  for (PropertyMap *propertyMap : refClassBase->getProperties())
    result +=
        QString(", %1.%2 as %1_%2").arg(tableAlias, propertyMap->getColumn());

  for (OneToOne *oneToOne : refClassBase->getOneToOneRelations())
    result += getOneToOneSelect(*refClassBase, oneToOne);

  return result;
}

QString SqlBuilderBase::getFrom(const Mapping::ClassMapBase &classBase) {
  QString result =
      QString("from %1 %2").arg(classBase.getTable()).arg(tableAlias);

  for (OneToOne *oneToOne : classBase.getOneToOneRelations())
    result += getOneToOneFrom(classBase, oneToOne, tableAlias);

  return result;
}

QString SqlBuilderBase::getOneToOneFrom(const Mapping::ClassMapBase &classBase,
                                        OneToOne *oneToOne,
                                        const QString &mainTableAlias) {
  QString result = "";

  QString tableAliasOneToOne = oneToOneAlias.value(oneToOne);

  QString property = oneToOne->getProperty();
  QString refClass = getTypeNameOfProperty(classBase.getMetaObject(), property);
  Mapping::ClassMapBase *refClassBase =
      ConfigurationMap::getMappedClass(refClass);

  result += QString(" left join %1 %2 on %2.%3 = %4.%5")
                .arg(refClassBase->getTable())
                .arg(tableAliasOneToOne)
                .arg(refClassBase->getIdProperty().getColumn())
                .arg(mainTableAlias)
                .arg(oneToOne->getTableColumn());

  for (OneToOne *oneToOne : refClassBase->getOneToOneRelations())
    result += getOneToOneFrom(*refClassBase, oneToOne, tableAliasOneToOne);

  return result;
}

QString SqlBuilderBase::getWhere(const QString &tableName,
                                 const GroupConditions &conditions) const {
  QString whereClause;
  for (Condition *f : conditions.getFilters()) {
    QString groupOp = whereClause.isEmpty()
                          ? ""
                          : groupOperationToString(conditions.getOperation());

    if (f->getOperation() == Operation::Like) {
      whereClause += QString("%1 %2 ").arg(groupOp).arg(
          getLikeCondition(tableName, f->getFieldName()));
    } else {
      if (f->getOperation() == Operation::NotEqual &&
          (f->getValues().first().isNull() ||
           !f->getValues().first().isValid())) {
        if (tableName.isEmpty())
          whereClause +=
              QString("%1 %2 is not null ").arg(groupOp).arg(f->getFieldName());
        else
          whereClause += QString("%1 %2.%3 is not null ")
                             .arg(groupOp)
                             .arg(tableName)
                             .arg(f->getFieldName());
      } else if (tableName.isEmpty())
        whereClause += QString("%1 %2 %4 :%2 ")
                           .arg(groupOp)
                           .arg(f->getFieldName())
                           .arg(operationToString(*f));
      else
        whereClause += QString("%1 %2.%3 %4 :%3 ")
                           .arg(groupOp)
                           .arg(tableName)
                           .arg(f->getFieldName())
                           .arg(operationToString(*f));
    }
  }

  for (GroupConditions *group : conditions.getGroups()) {
    QString groupOp = whereClause.isEmpty()
                          ? ""
                          : groupOperationToString(conditions.getOperation());
    QString groupWhere = getWhere(tableName, *group);
    if (!groupWhere.isEmpty())
      whereClause += QString("%1 (%2)").arg(groupOp).arg(groupWhere);
  }
  return whereClause;
}

QString SqlBuilderBase::operationToString(const Condition &filter) const {
  QString operationString;
  Operation operation = filter.getOperation();
  if (operation == Operation::Equal) {
    operationString = "=";
  } else if (operation == Operation::NotEqual) {
    if (filter.getValues().first().isNull() ||
        !filter.getValues().first().isValid())
      operationString = "is not";
    else
      operationString = "!=";
  } else if (operation == Operation::Like) {
    operationString = "like";
  }
  return operationString;
}

QString
SqlBuilderBase::groupOperationToString(GroupOperation groupOperation) const {
  return groupOperation == GroupOperation::And ? "and" : "or";
}

void SqlBuilderBase::bindValues(QSqlQuery &query, const GroupConditions &conditions) {
  for (Condition *f : conditions.getFilters()) {
    if (!f->getValues().isEmpty()) {
      query.bindValue(getPlaceHolder(f->getFieldName()),
                      f->getValues().first());
    }
  }
  for (GroupConditions *g : conditions.getGroups()) {
    bindValues(query, *g);
  }
}

void SqlBuilderBase::setOneToOneAlias(const QMap<OneToOne *, QString> &value) {
  oneToOneAlias = value;
}

void SqlBuilderBase::fillOneToOneAlias() {
  for (OneToOne *oneToOne : classBase->getOneToOneRelations())
    fillOneToOneAlias(*classBase, oneToOne);
}

void SqlBuilderBase::fillOneToOneAlias(const ClassMapBase &classBase,
                                       OneToOne *oneToOne) {
  if (!oneToOneAlias.contains(oneToOne))
    oneToOneAlias.insert(oneToOne, generateTableAlias());

  QString property = oneToOne->getProperty();
  QString refClass = SqlBuilderBase::getTypeNameOfProperty(
      classBase.getMetaObject(), property);
  Mapping::ClassMapBase *refClassBase =
      ConfigurationMap::getMappedClass(refClass);
  for (OneToOne *oneToOne : refClassBase->getOneToOneRelations())
    fillOneToOneAlias(*refClassBase, oneToOne);
}

QString SqlBuilderBase::generateTableAlias() {
  return tableAliasTemplate.arg(++tableNumber);
}

QString SqlBuilderBase::getCurrentTableAlias() const {
  return tableAliasTemplate.arg(tableNumber);
}

void SqlBuilderBase::resetTableNumber() { tableNumber = 0; }

QMap<OneToOne *, QString> SqlBuilderBase::getOneToOneAlias() const {
  return oneToOneAlias;
}

QSqlDatabase SqlBuilderBase::getDatabase() const { return database; }

void SqlBuilderBase::setDatabase(const QSqlDatabase &value) {
  database = value;
}

QObject *SqlBuilderBase::getObject() const { return object; }

void SqlBuilderBase::setObject(QObject *value) { object = value; }

ClassMapBase *SqlBuilderBase::getClassBase() const { return classBase; }

void SqlBuilderBase::setClassBase(ClassMapBase *value) { classBase = value; }

QString SqlBuilderBase::getTypeNameOfProperty(const QObject &obj,
                                              const QString &prop) {
  return getTypeNameOfProperty(*obj.metaObject(), prop);
}

QString SqlBuilderBase::getTypeNameOfProperty(const QMetaObject &meta,
                                              const QString &prop) {
  int propertyIndex = meta.indexOfProperty(prop.toStdString().data());
  QMetaProperty metaProperty = meta.property(propertyIndex);
  QString refClass = metaProperty.typeName();
  if (refClass.right(1) == "*")
    refClass = refClass.left(refClass.size() - 1);

  return refClass;
}

QString SqlBuilderBase::getTableAlias() const { return tableAlias; }

void SqlBuilderBase::setTableAlias(const QString &value) { tableAlias = value; }

GroupConditions SqlBuilderBase::getConditions() const { return conditions; }

void SqlBuilderBase::setConditions(const GroupConditions &value) { conditions = value; }
}
}
