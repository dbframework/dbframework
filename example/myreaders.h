#ifndef MYREADERS_H
#define MYREADERS_H

#include "dbframework.h"
#include "testtypes.h"
#include <QSqlQuery>
#include <QString>


class Reader2Customer : public dbframework::DBReader2Object<QSqlQuery, Customer> {
public:
    Reader2Customer() : DBReader2Object<QSqlQuery, Customer>(){};
    Reader2Customer(Customer* c) : DBReader2Object<QSqlQuery, Customer>(c){};
    bool read(QSqlQuery& ds);
};

class Reader2Account : public dbframework::DBReader2Object<QSqlQuery, Account> {
public:
    Reader2Account() : DBReader2Object<QSqlQuery, Account>(){};
    Reader2Account(Account* a) : DBReader2Object<QSqlQuery, Account>(a){};
    bool read(QSqlQuery& ds);
};

class Reader2Transaction : public dbframework::DBReader2Object<QSqlQuery, Transaction> {
public:
    Reader2Transaction() : DBReader2Object<QSqlQuery, Transaction>(){};
    Reader2Transaction(Transaction* t) : DBReader2Object<QSqlQuery, Transaction>(t){};
    bool read(QSqlQuery& ds);
};

class Reader2AccountWithTrans : public dbframework::DBReader2Object<QSqlQuery, AccountWithTrans> {
private:
    Reader2Account readAccount;
    Reader2Transaction readTransaction;    
    dbframework::DBReader2STLContainerPtr<QSqlQuery, Transaction, TransactionVector, TransactionPtr> readTransactions;
public:
    Reader2AccountWithTrans() : DBReader2Object<QSqlQuery, AccountWithTrans>(){};
    Reader2AccountWithTrans(AccountWithTrans* a) : DBReader2Object<QSqlQuery, AccountWithTrans>(a){};
    bool read(QSqlQuery& ds);
};

class QKeyReader : public dbframework::DBReader2Object<QSqlQuery, int> {
public:
    QString field;
    QKeyReader() : DBReader2Object<QSqlQuery, int>(){};
    QKeyReader(int* obj) : DBReader2Object<QSqlQuery, int>(obj){};
    bool read(QSqlQuery& ds);
};

class Reader2CustomerFullInfo : public dbframework::DBReader2Object<QSqlQuery, CustomerFullInfo> {
private:
    Reader2Customer readCustomer;
    Reader2AccountWithTrans readAccount;
    QKeyReader readKey;
    dbframework::DBReader2STLAssosiativePtr<QSqlQuery, AccountWithTrans, AccountWithTransMap,
        int, AccountWithTransPtr> readAccounts;
public:
    Reader2CustomerFullInfo() : DBReader2Object<QSqlQuery, CustomerFullInfo>(){};
    Reader2CustomerFullInfo(CustomerFullInfo* c) : DBReader2Object<QSqlQuery, CustomerFullInfo>(c){};
    bool read(QSqlQuery& ds);
};

#endif // MYREADERS_H
