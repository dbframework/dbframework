#include "mybinders.h"
#include <QVariant>

void QDBBind::bind(QSqlQuery& dataset)
{
    dataset.bindValue(m_name, m_value);
}

void CustomerBinder::bind(QSqlQuery& dataset)
{
    dataset.bindValue(":customer_id", m_object->id);
    dataset.bindValue(":customer_name", QString::fromStdWString(m_object->name));
}

void AccountBinder::bind(QSqlQuery& dataset)
{
    dataset.bindValue(":account_id", m_object->id);
    dataset.bindValue(":account_ref_customer", m_object->ref_customer);
}

void TransactionBinder::bind(QSqlQuery& dataset)
{
    dataset.bindValue(":tran_id", m_object->id);
    dataset.bindValue(":tran_ref_account", m_object->ref_account);
    dataset.bindValue(":tran_amount", m_object->amount);
}

