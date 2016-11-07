#ifndef DML_H
#define DML_H

#include <QString>

QString sqlCreatePg = "create table TypeA(id serial primary key, code varchar, name varchar);"
                      "create table KindA(id serial primary key, code varchar, name varchar, "
                      "idTypeA integer constraint TypeA_FK "
                      "references TypeA(id));"
                      "create table A(id serial primary key, code varchar, idKindA integer "
                      "constraint KindA_FK references KindA(id));"
                      "create table B(id serial primary key, idA integer constraint A_FK "
                      "references KindA(id), code varchar);";

QStringList sqlCreateSqlite{
    "create table TypeA(id integer primary key autoincrement, code text, name "
    "text)",
    "create table KindA(id integer primary key autoincrement, code text, name "
    "text, idTypeA integer, foreign key (idTypeA) references TypeA(id))",
    "create table A(id integer primary key autoincrement, code text, idKindA "
    "integer, foreign key (idKindA) references KindA(id))",
    "create table B(id integer primary key autoincrement, idA integer, code "
    "text, foreign key (idA)  references KindA(id))",
    "create table C(id integer primary key autoincrement, idKindA integer, foreign key (idKindA)  references "
    "KindA(id))",
    "create table D(id integer primary key autoincrement, idKindA integer, foreign key (idKindA)  references "
    "KindA(id))",
    "create table E(id integer primary key autoincrement, idC integer, idD integer, foreign key (idC)  references "
    "C(id), foreign key (idD)  references D(id))"};

QStringList sqlFill{"insert into TypeA(code, name) values('code1', 'value1');",
                    "insert into TypeA(code, name) values('code2', 'value2');",
                    "insert into KindA(code, name, idTypeA) values('code1', 'value1', 1);",
                    "insert into KindA(code, name, idTypeA) values('code2', 'value2', 2);",
                    "insert into A(code, idKindA) values('code1', 1);",
                    "insert into A(code, idKindA) values('code2', 2);",
                    "insert into B(idA, code) values(1, 'code1.1');",
                    "insert into B(idA, code) values(1, 'code1.2');",
                    "insert into B(idA, code) values(2, 'code2.1');",
                    "insert into B(idA, code) values(2, 'code2.2');",
                    "insert into B(idA, code) values(2, 'code2.3');",
                    "insert into C(idKindA) values(1);",
                    "insert into D(idKindA) values(2);",
                    "insert into E(idC, idD) values(1, 1);"};

#endif // DML_H
