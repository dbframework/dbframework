#ifndef MYBINDERS_H
#define MYBINDERS_H

#include "dbframework.h"
#include "testtypes.h"
#include <QSqlQuery>
#include <QVariant>

class QDBBind : public dbframework::DBBind<QSqlQuery, QString, QVariant> {
public:
    QDBBind(const QString& name, const QVariant& value):DBBind<QSqlQuery, QString, QVariant>(name, value){};
    void bind(QSqlQuery& dataset);
};

typedef dbframework::DBBinders<QSqlQuery> QDBBinders;

class CustomerBinder : public dbframework::DBBindObject<QSqlQuery, Customer> {
public:
    CustomerBinder(Customer* c):DBBindObject<QSqlQuery, Customer>(c){};
    void bind(QSqlQuery& dataset);
};

class AccountBinder : public dbframework::DBBindObject<QSqlQuery, Account> {
public:
    AccountBinder(Account* acc):DBBindObject<QSqlQuery, Account>(acc){};
    void bind(QSqlQuery& dataset);
};

class TransactionBinder : public dbframework::DBBindObject<QSqlQuery, Transaction> {
public:
    TransactionBinder(Transaction* t):DBBindObject<QSqlQuery, Transaction>(t){};
    void bind(QSqlQuery& dataset);
};


#endif // MYBINDERS_H
