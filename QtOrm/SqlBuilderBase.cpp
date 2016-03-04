#include "SqlBuilderBase.h"
#include "ClassMapBase.h"
#include "NullableBase.h"
#include <QString>
#include <QSqlError>
#include <QSqlRecord>
#include <QMetaProperty>

namespace QtOrm {
namespace Config {
extern QMap<QString, QtOrm::Mapping::ClassMapBase *> mappedClass;
}

namespace Sql {
SqlBuilderBase::SqlBuilderBase(const QSqlDatabase &database, QObject *parent)
    : QObject(parent), database(database), tableNumber(0) {
}

QObject *SqlBuilderBase::getById(const QString &className, const QVariant &id) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(className);

  auto list = getListObject(className, classBase->getIdProperty().getName(), id);

  if (list->size() == 0)
    throw new Exception("Не найдено ни одной записи.");

  if (list->size() > 1)
    throw new Exception(QString("Найдено %1 записей с идентификатором '%2'").arg(list->size()).arg(id.toString()));

  return list->takeFirst();
}

QList<QObject *> *
SqlBuilderBase::getListObject(const QString &className, const QString property, const QVariant value) {
  Group group;
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(className);
  if (!property.isEmpty()) {
    QString column = classBase->getProperty(property).getColumn();
    Filter *filter = new Filter();
    filter->setFieldName(column);
    filter->setOperation(Operation::Equal);
    QList<QVariant> values;
    values.append(value);
    filter->setValues(values);
    group.addFilter(filter);
  }

  return getListObject(className, group);
}

QList<QObject *> *SqlBuilderBase::getListObject(const QString &className, const Group &conditions) {
  checkClass(className);

  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(className);
  resetTableNumber();
  QString tableName = generateTableAlias();
  QString select = getSelect();
  QString from = getFrom(classBase->getTable());
  QString where;
  if (!conditions.isEmpty())
    where = QString("where %1").arg(getWhere(tableName, conditions));
  QString fullSqlText = sqlQueryTemplate.arg(select).arg(from).arg(where);

  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindValues(query, conditions);

  if (!query.exec()) {
    emit executeSql(getSqlTextWithBindParams(query));
    throw new QtOrm::Exception(query.lastError().text());
  }
  emit executeSql(getSqlTextWithBindParams(query));

  return getList(*classBase, query);
}

QString SqlBuilderBase::generateTableAlias() {
  return tableAliasTemplate.arg(++tableNumber);
}

QString SqlBuilderBase::getCurrentTableAlias() const {
  return tableAliasTemplate.arg(tableNumber);
}

void SqlBuilderBase::resetTableNumber() {
  tableNumber = 0;
}

QString SqlBuilderBase::getPlaceHolder(const QString param) {
  return QString(":%1").arg(param);
}

QString SqlBuilderBase::getSqlTextWithBindParams(QSqlQuery &query) {
  QString text;
  text = query.lastQuery() + "\n";

  auto boundValues = query.boundValues();
  for (auto it = boundValues.begin(); it != boundValues.end(); ++it)
    text += QString("%1 = %2\n").arg(it.key()).arg(it.value().toString());

  return text;
}

QVariant SqlBuilderBase::prepareValue(QVariant &value) {
  if (value.canConvert(QMetaType::type("NullableBase")))
    return value.value<NullableBase>().getVariant();

  return value;
}

QString SqlBuilderBase::getLikeCondition(const QString &tableName, const QString &fieldName) const {
  return QString("%1.%2 like '%' || :%2 || '%'").arg(tableName).arg(fieldName);
}

QString SqlBuilderBase::getSelect() const {
  return QString("select %1.*").arg(getCurrentTableAlias());
}

QString SqlBuilderBase::getFrom(const QString &tableName) const {
  return QString("from %1 %2").arg(tableName).arg(getCurrentTableAlias());
}

QString SqlBuilderBase::getWhere(const QString &tableName, const Group &conditions) const {
  QString whereClause;
  for (Filter *f : conditions.getFilters()) {
    QString groupOp = whereClause.isEmpty() ? "" : groupOperationToString(conditions.getOperation());

    if (f->getOperation() == Operation::Like) {
      whereClause += QString("%1 %2").arg(groupOp).arg(getLikeCondition(tableName, f->getFieldName()));
    } else {
      whereClause += QString("%1 %2.%3 %4 :%3")
                         .arg(groupOp)
                         .arg(tableName)
                         .arg(f->getFieldName())
                         .arg(operationToString(f->getOperation()));
    }
  }

  for (Group *group : conditions.getGroups()) {
    QString groupOp = whereClause.isEmpty() ? "" : groupOperationToString(conditions.getOperation());
    whereClause += QString("%1 (%2)").arg(groupOp).arg(getWhere(tableName, *group));
  }
  return whereClause;
}

QString SqlBuilderBase::operationToString(Operation operation) const {
  QString operationString;
  if (operation == Operation::Equal) {
    operationString = "=";
  } else if (operation == Operation::NotEqual) {
    operationString = "!=";
  } else if (operation == Operation::Like) {
    operationString = "like";
  }
  return operationString;
}

QString SqlBuilderBase::groupOperationToString(GroupOperation groupOperation) const {
  return groupOperation == GroupOperation::And ? "and" : "or";
}

void SqlBuilderBase::bindValues(QSqlQuery &query, const Group &conditions) {
  for (Filter *f : conditions.getFilters()) {
    if (!f->getValues().isEmpty()) {
      query.bindValue(getPlaceHolder(f->getFieldName()), f->getValues().first());
    }
  }
  for (Group *g : conditions.getGroups()) {
    bindValues(query, *g);
  }
}

QList<QObject *> *SqlBuilderBase::getList(Mapping::ClassMapBase &classBase, QSqlQuery &query) {
  QList<QObject *> *objects = new QList<QObject *>();
  while (query.next()) {
    QObject *obj = classBase.getMetaObject().newInstance();
    fillObject(classBase, query.record(), *obj);
    fillOneToMany(classBase.getOneToManyRelations(), classBase.getIdProperty().getName(), *obj);
    fillOneToOne(classBase, *obj);
    objects->append(obj);
  }

  return objects;
}

void SqlBuilderBase::checkClass(const QString &className) {
  if (!Config::ConfigurateMap::isRegisterClass(className))
    throw new Exception(QString("Класс '%1' не зарегистрирован.").arg(className));
}

void SqlBuilderBase::fillObject(const Mapping::ClassMapBase &classBase, const QSqlRecord &record, QObject &object) {
  auto properties = classBase.getProperties();
  foreach (auto prop, properties) {
    QVariant value = record.value(prop->getColumn());
    int propertyIndex = object.metaObject()->indexOfProperty(prop->getName().toStdString().c_str());
    QMetaProperty metaProperty = object.metaObject()->property(propertyIndex);
    if (QString(metaProperty.typeName()) == "NullableBase") {
      NullableBase *tmpValNullable = new NullableBase();
      tmpValNullable->setVariant(value);
      value = QVariant::fromValue(*tmpValNullable);
    }
    bool success = object.setProperty(prop->getName().toStdString().c_str(), value);
    if (!success) {
      throw new Exception(
          QString("Неудалось поместить значние в %1.%2").arg(classBase.getClassName()).arg(prop->getName()));
    }
  }
}

void SqlBuilderBase::fillOneToMany(const QMap<QString, Mapping::OneToMany *> &relations,
                                   const QString &idProperty,
                                   QObject &object) {
  foreach (auto oneToMany, relations) {
    QString refClass = oneToMany->getRefClass();
    QString property = oneToMany->getProperty();
    QString refProperty = oneToMany->getRefProperty();
    QVariant value = object.property(idProperty.toStdString().data());
    QList<QObject *> *l = getListObject(refClass, refProperty, value);
    QVariant var = QVariant::fromValue(*l);
    objectSetProperty(object, property.toStdString().data(), var);
  }
}

void SqlBuilderBase::fillOneToOne(Mapping::ClassMapBase &classBase, QObject &object) {
  const QMap<QString, Mapping::OneToOne *> &relations = classBase.getOneToOneRelations();
  foreach (auto oneToOne, relations) {
    QString property = oneToOne->getProperty();
    int propertyIndex = object.metaObject()->indexOfProperty(property.toStdString().data());
    QMetaProperty metaProperty = object.metaObject()->property(propertyIndex);
    QString refClass = metaProperty.typeName();
    if (refClass.right(1) == "*")
      refClass = refClass.left(refClass.size() - 1);
    Mapping::ClassMapBase *refClassBase = Config::ConfigurateMap::getMappedClass(refClass);
    QString fullSqlText = QString("select t2.* "
                                  "from %1 t1 join %2 t2 on (t1.%3 = t2.%4) "
                                  "where t1.%5 = :id")
                              .arg(classBase.getTable())
                              .arg(refClassBase->getTable())
                              .arg(oneToOne->getTableColumn())
                              .arg(refClassBase->getIdProperty().getColumn())
                              .arg(classBase.getIdProperty().getColumn());

    QSqlQuery query(database);
    query.prepare(fullSqlText);

    QVariant curObjIdVal = object.property(classBase.getIdProperty().getName().toStdString().data());
    query.bindValue(":id", curObjIdVal);

    if (!query.exec()) {
      getSqlTextWithBindParams(query);
      throw new QtOrm::Exception(query.lastError().text());
    }
    getSqlTextWithBindParams(query);

    QList<QObject *> *lst = getList(*refClassBase, query);

    if (lst->count() != 0) {
      QVariant var = QVariant::fromValue(lst->first());
      objectSetProperty(object, property.toStdString().data(), var);
    }
  }
}

void SqlBuilderBase::objectSetProperty(QObject &object, const char *propertyName, const QVariant &value) {
  if (!object.setProperty(propertyName, value)) {
    QString textError = QString("Произошла ошибка при присваивании значения свойству '%1.%2'")
                            .arg(object.metaObject()->className())
                            .arg(propertyName);
    throw new Exception(textError);
  }
}

//QTextStream *SqlBuilderBase::getTextStream() const {
//  return textStream;
//}

//void SqlBuilderBase::setTextStream(QTextStream *value) {
//  textStream = value;
//}
}
}
