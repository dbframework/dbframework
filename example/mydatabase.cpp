#include "mydatabase.h"
#include <QSqlQuery>
#include <QTime>
#include <QVariant>
#include "mybinders.h"
#include "mydescriptors.h"

using namespace dbframework;

const char* scriptDBCreate[] = {
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

bool MySQLExec::executeQuery(QSqlQuery& ds)
{
    return ds.exec();
}

bool MySQLExec::next(QSqlQuery& ds)
{
    return ds.next();
}

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

bool MyDatabase::execScript(const char** script)
{
    bool result = true;
    for(const char** sql = script; (*sql != nullptr) && result; ++sql){
        result = execSql(QString(*sql));
    }
    return result;
}

bool MyDatabase::execSql(QString sql)
{    
    return execSql(sql.toStdWString().c_str(), nullptr, nullptr);
}

bool MyDatabase::fillDB(int customerCnt, int accountCnt,
                        int tranCnt)
{
    bool result = true;
    DBSQLGeneratorImpl<std::string, size_t> SQLGen;
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    QSqlQuery q(db);

    if (result)
        result = db.transaction();

    if (result) {
        //fill customer        
        Customer c;
        for (int i = 0; (i < customerCnt) && result; ++i) {            
            c.id = i;
            c.name = QString(QString("customer") + QString::number(i)).toStdWString();            
            result = execSql(SQLGen.insert(&DescriptorCustomer, true).c_str(), nullptr, &CustomerBinder(&c));
        }
    }

    if (result) {
        Account acc;
        //fill account        
        for (int i = 0; (i < accountCnt) && result; ++i) {            
            acc.id = i;
            acc.ref_customer = qrand() % customerCnt;
            result = execSql(SQLGen.insert(&DescriptorAccount, true).c_str(), nullptr, &AccountBinder(&acc));
        }
    }

    if (result) {
        //fill tran
        Transaction t;
        for (int i = 0; (i < tranCnt) && result; ++i) {
            t.id = i;
            t.ref_account = qrand() % accountCnt;
            t.amount = 1 + qrand() % 10;
            result = execSql(SQLGen.insert(&DescriptorTransaction, true).c_str(), nullptr, &TransactionBinder(&t));
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

bool MyDatabase::execSql(const wchar_t *sql, DBReader<QSqlQuery>* reader, DBBinder<QSqlQuery>* binder)
{
    QSqlQuery q(db);
    bool result = q.prepare(QString::fromStdWString(sql));

    if (result)
        result = MySQLExec().exec(q, binder, reader);

    return result;
}

bool MyDatabase::execSql(const char *sql, DBReader<QSqlQuery>* reader, DBBinder<QSqlQuery>* binder)
{
    QSqlQuery q(db);
    bool result = q.prepare(QString::fromStdString(sql));

    if (result)
        result = MySQLExec().exec(q, binder, reader);

    return result;
}
