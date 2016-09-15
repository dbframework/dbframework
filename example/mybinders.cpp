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

