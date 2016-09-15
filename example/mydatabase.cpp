#include "mydatabase.h"
#include <QSqlQuery>
#include <QTime>
#include <QVariant>

using namespace dbframework;

char* scriptDBCreate[] = {
    "drop table if exists tran",

    "drop table if exists account",

    "drop table if exists customer",

    "create table customer (\
    id integer primary key,\
    name text not null)",

    "create table account (\
    id integer primary key,\
    ref_customer integer not null,\
    constraint customer_fk foreign key(ref_customer) references customer(id) \
    on update restrict on delete restrict)",

    "create table tran (\
    id integer primary key,\
    ref_account integer not null,\
    amount integer not null,\
    constraint account_fk foreign key(ref_account) references account(id) \
    on update restrict on delete restrict)",

    nullptr
};

char* scriptDBFill[] = {    
    "insert into customer values(:id, :name)",
    "insert into account values(:id, :ref_customer)",
    "insert into tran values(:id, :ref_account, :amount)",
    nullptr
};

MyDatabase::MyDatabase()
{
}

bool MyDatabase::open(wchar_t* fileName)
{
    bool result = false;

    db = QSqlDatabase::addDatabase("QSQLITE", "dbframeworkDB");
    db.setDatabaseName(QString::fromStdWString(fileName));
    result = db.open();
    if (result) {
        result = execScript(scriptDBCreate);
        if (result)
            result = fillDB(10, 20, 100);
    }

    return result;
}

bool MyDatabase::execScript(char** script)
{
    bool result = true;
    for(char** sql = script; (*sql != nullptr) && result; ++sql){
        result = execSql(QString(*sql));
    }
    return result;
}

bool MyDatabase::execSql(QString sql)
{
    QSqlQuery q(db);

    bool result = q.prepare(sql);
    if (result)
        result = q.exec();
    return result;
}

bool MyDatabase::fillDB(int customerCnt, int accountCnt,
                        int tranCnt)
{
    bool result = true;
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    QSqlQuery q(db);

    if (result)
        result = db.transaction();

    if (result) {
        //fill customer
        result = q.prepare(QString(scriptDBFill[0]));
        for (int i = 0; (i < customerCnt) && result; ++i) {
            q.bindValue(":id", i);
            q.bindValue(":name", "customer" + QString::number(i));
            result = q.exec();
        }
    }

    if (result) {
        //fill account
        result = q.prepare(QString(scriptDBFill[1]));
        for (int i = 0; (i < accountCnt) && result; ++i) {
            q.bindValue(":id", i);
            q.bindValue(":ref_customer", qrand() % customerCnt);
            result = q.exec();
        }
    }

    if (result) {
        //fill tran
        result = q.prepare(QString(scriptDBFill[2]));
        for (int i = 0; (i < tranCnt) && result; ++i) {
            q.bindValue(":id", i);
            q.bindValue(":ref_account", qrand() % accountCnt);
            q.bindValue(":amount", qrand() % 10);
            result = q.exec();
        }
    }

    if (result) {
        db.commit();
    }
    else {
        db.rollback();
    }

    return result;
}

bool MyDatabase::execSql(wchar_t *sql, DBReader<QSqlQuery>* reader, DBBinder<QSqlQuery>* binder)
{
    QSqlQuery q(db);
    bool result = true;

    if(result)
        result = q.prepare(QString::fromStdWString(sql));
    if (result && (binder != nullptr))
        binder->bind(q);
    if (result)
        result = q.exec();
    for (;(reader != nullptr) && result && q.next();) {
        result = reader->read(q);
    }
    return result;
}
