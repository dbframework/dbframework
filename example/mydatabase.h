#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QString>
#include <QSqlDatabase>
#include "dbframework.h"

class MyDatabase
{
private:
    QSqlDatabase db;
    bool execScript(char** script);
    bool execSql(QString sql);
    bool fillDB(int customerCnt, int accountCnt, int tranCnt);
public:
    MyDatabase();
    bool open(wchar_t* fileName);
    bool execSql(wchar_t* sql, dbframework::DBReader<QSqlQuery>* reader, dbframework::DBBinder<QSqlQuery>* binder);
};

#endif // MYDATABASE_H
