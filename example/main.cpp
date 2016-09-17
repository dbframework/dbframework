#include <iostream>
#include "mydatabase.h"
#include "testtypes.h"
#include "myreaders.h"
#include "mybinders.h"

using namespace std;
using namespace dbframework;

MyDatabase db;

bool customer(int id, Customer& result)
{
    bool res ;
    Reader2Customer reader(&result);
    QDBBind binder(":id", id);
    wchar_t query[] = L"select id as customer_id, name as customer_name from customer where id = :id";
    res = db.execSql(query, &reader, &binder);
    return res;
}

bool customer(int idmin, int idmax, CustomerVector& v)
{
    bool result = false;
    Reader2Customer rc;
    DBReader2Container<QSqlQuery, Customer, CustomerVector> r(&v, &rc);
    DBBinders<QSqlQuery> binder(new QDBBind(":id_min", idmin), new QDBBind(":id_max", idmax), true);
    wchar_t query[] = L"select id as customer_id, name as customer_name from customer where id >= :id_min and id <= :id_max";
    result = db.execSql(query, &r, &binder);
    return result;
}

bool customer(CustomerFullInfo& c)
{
    bool result = false;
    Reader2CustomerFullInfo rc(&c);
    CustomerBinder binder(&c);
    wchar_t query[] = L"select c.id as customer_id, c.name as customer_name, \
            a.id as account_id, a.ref_customer as account_ref_customer, \
            t.id as tran_id, t.ref_account as tran_ref_account, t.amount as tran_amount \
            from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account \
            where c.id = :customer_id";
    result =  db.execSql(query, &rc, &binder);
    return result;
}

void printUsage()
{
    wcout << "Usage: example <database file name>" << endl;
}

bool openDatabase(wchar_t *fileName)
{
    return db.open(fileName);
}

bool printQueries()
{
    bool result = false;
    Customer c;
    CustomerVector v;
    CustomerFullInfo cf;

    result = customer(5, c);

    if (result) {
        wcout << "select id as customer_id, name as customer_name from customer where id = 5" << endl;
        wcout << c;
        result = customer(2, 6, v);
    }

    if (result) {
        wcout << L"select id as customer_id, name as customer_name from customer where id >= 2 and id <= 6" << endl;
        wcout << v;
        cf.id = 5;
        result = customer(cf);
    }

    if (result) {
        wcout << L"select c.id as customer_id, c.name as customer_name, \
                a.id as account_id, a.ref_customer as account_ref_customer, \
                t.id as tran_id, t.ref_account as tran_ref_account, t.amount as tran_amount \
                from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account \
                where c.id = 5" << endl;
        wcout << cf;
    }
    return result;
}

int wmain(int argc, wchar_t *argv[])
{
    int result = 0;

    if (argc < 2) {
        result = 1;
        printUsage();
    }
    else {
        if (openDatabase(argv[1])) {
            if (!printQueries()) {
                result = 3;
            }
        }
        else {
            result = 2;
        }
    }

    return result;
}


