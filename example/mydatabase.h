#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QString>
#include <QSqlDatabase>
#include "dbframework.h"

class MySQLExec : public dbframework::DBSQLExecutor<QSqlQuery>
{
protected:
    bool executeQuery(QSqlQuery& ds);
    bool next(QSqlQuery& ds);
};

class MyDatabase
{
private:
    QSqlDatabase db;
    bool execScript(const char **script);
    bool execSql(QString sql);
    bool fillDB(int customerCnt, int accountCnt, int tranCnt);
public:
    MyDatabase();
    bool open(wchar_t* fileName);
    bool execSql(const wchar_t* sql, dbframework::DBReader<QSqlQuery>* reader, dbframework::DBBinder<QSqlQuery>* binder);
    bool execSql(const char* sql, dbframework::DBReader<QSqlQuery>* reader, dbframework::DBBinder<QSqlQuery>* binder);
};

#endif // MYDATABASE_H
