#include "myreaders.h"
#include <QVariant>
#include <QSqlQuery>


bool checkIdField(QString name, QSqlQuery &ds, int* id)
{
    QVariant v = ds.value(name);
    bool result =(v.isValid() && !v.isNull());
    if (result)
        *id = v.toInt();
    return result;
}

bool Reader2Customer::read(QSqlQuery& ds)
{
    bool result = checkIdField("customer_id", ds, &m_object->id);
    if (result)
        m_object->name = ds.value("customer_name").toString().toStdWString();
    return result;
}

bool Reader2Account::read(QSqlQuery& ds)
{
    bool result = checkIdField("account_id", ds, &m_object->id);
    if (result)
        m_object->ref_customer = ds.value("account_ref_customer").toInt();
    return result;
}

bool Reader2Transaction::read(QSqlQuery& ds)
{
    bool result = checkIdField("tran_id", ds, &m_object->id);
    if (result) {
        m_object->ref_account = ds.value("tran_ref_account").toInt();
        m_object->amount = ds.value("tran_amount").toInt();
    }
    return result;
}

bool Reader2AccountWithTrans::read(QSqlQuery& ds)
{
    readAccount.setObject(&m_object->account);
    readTransactions.setObject(&m_object->transactions);
    readTransactions.setReader(&readTransaction);

    bool result = readAccount.read(ds);
    if (result) {
        readTransactions.read(ds);
    }
    return result;
}

bool QKeyReader::read(QSqlQuery &ds)
{
    return checkIdField(field, ds, m_object);
}

bool Reader2CustomerFullInfo::read(QSqlQuery &ds)
{
    readCustomer.setObject(m_object);
    readAccounts.setObject(&m_object->accounts);
    readAccounts.setReader(&readAccount);
    readAccounts.setKeyReader(&readKey);
    readKey.field = "account_id";

    bool result = readCustomer.read(ds);
    if (result) {
        readAccounts.read(ds);
    }

    return result;
}
