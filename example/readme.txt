Introduction

This is an example of using dbframework for creating application that executes SQL queries. This example uses SQLite database.  Qt SDK version 5.2.0 or higher is required to build this example. The project file is example.pro.

The result of building this example is a console application. The application accepts one parameter - database file name. The application creates database, fills it and outputs to the screen results of execution of 3 SQL queries. 

Next sections describe the creation of the example application. The following notation is used. Database object names (names of tables, fields etc.) and SQL query texts are enclosed in brackets []. С++ syntax elements (type names, variable name etc.) and source code fragments are enclosed in angle brackets <>.

1. Example database structure.

This example uses SQLite database with the data about customers, their accounts and transactions. Database contains 3 tables: [customer] (customer info), [account] (account info), [tran] (transaction info). The table [customer] is related with table [account] as one-to-many. The table [account] is related with the table [tran] as one-to-many.

The [customer] table contains information about customers. Table fields:
[id] - customer unique identifier;
[name] - customer name.

The [account] table contains information about accounts. Table fields:
[id] - account number;
[ref_customer] - account owner identifier in the [customer] table.

The [tran] table contains information about transactions. Table fields:
[id] - transaction unique identifier;
[ref_account] - account number in [account] table;
[amount] - transaction amount.

2. Querying one record from one table.

2.1 Problem statement.

Let's create function that will get the result of execution of the query:
[select * from customer where id = :id], where [:id] is a parameter. 

Customer data will be represented by class <Customer>:
<
class Customer {
public:
   int id;
   std::wstring name;
};
>.
In this example class fields are declared as public for simplicity. Nothing prevents declaring them private and using class methods to access them. Required function will be declared as <bool сustomer(int id, Customer& с)>.

The result of execution of a SQL query is a set of records. For reading this data using dbframework 3 components are required:
- class responsible for reading data from one record into the required data structure;
- class responsible for assigning query parameters values;
- function responsible for controlling query execution and data reading using mentioned two classes.

We'll create all this in the following sections.

2.2 Class for reading data.

The class for reading data must be <DBReader> descendant. In this case read data is stored into <Customer> instance, so class will be created by inheriting from <DBReader2Object>. <DBReader2Object> is a template class with parameters <Dataset> and <Object>. <Dataset> is a class used to perform query execution. In this example Qt library is used to access database, so <Dataset> will be <QSqlQuery> from Qt library. <Object> is a class which instance is used to store read data. Let's call our new class <Reader2Customer>:
<
class Reader2Customer : public dbframework::DBReader2Object<QSqlQuery, Customer> {
public:
    Reader2Customer() : DBReader2Object<QSqlQuery, Customer>(){};
    Reader2Customer(Customer* c) : DBReader2Object<QSqlQuery, Customer>(c){};
    bool read(QSqlQuery& ds);
};
>
<Reader2Customer> must implement abstract method <bool read(QSqlQuery& ds)>. Let's do this in the following way:
<
bool Reader2Customer::read(QSqlQuery& ds)
{
    //read id field
    m_object->id = ds.value("id").toInt();
    //read name field
    m_object->name = ds.value("name").toString().toStdWString();    
    return true;
}
>.
In this simple implementation correctness of reading isn't checked.

2.3 Class for binding parameters to values.

Class for binding parameters must be <DBBinder> descendant. In this query has only one parameter, so the class will be created by inheriting from <DBBind>. <DBBind> is a class template with parameters <Dataset>, <NameType>, <ValueType>. <DBBind> is designed to bind one parameter to its value by parameter name. <Dataset> is a class used to perform query execution. In this case it is <QSqlQuery>. <NameType> is a type of parameter name. Let's use <QString> - dynamic string from Qt library. <ValueType> is a type of the parameter value. In this case <QVariant> will be used. <QVariant> is the class which can store values of different types from Qt library. Let's call our new class <QDBBind>:
<
class QDBBind : public dbframework::DBBind<QSqlQuery, QString, QVariant> {
public:
    QDBBind(const QString& name, const QVariant& value):DBBind<QSqlQuery, QString, QVariant>(name, value){};
    void bind(QSqlQuery& dataset);
};
>
<QDBBind> must implement abstract method <void bind(QSqlQuery& ds)>. Let's do this in the following way:
void QDBBind::bind(QSqlQuery& dataset)
{
    //m_name, m_value - protected fields inherited from DBBind. Their values are passed to the constructor.
    //Bind parameter m_name to m_value.
    dataset.bindValue(m_name, m_value);
}
>.

2.4 Fuction for data reading control.

In this example the function for data reading control is implemented as <MyDatabase> class member:
<
/*
    sql - SQL-query text.
    reader - instance of the class for reading data.
    binder - instance of the class for binding parameters.
*/
bool MyDatabase::execSql(wchar_t *sql, DBReader<QSqlQuery>* reader, DBBinder<QSqlQuery>* binder)
{
    //Create object for query execution linked with previously opened database.
    QSqlQuery q(db);
    bool result = true;

    //Prepare the query.
    if(result)
        result = q.prepare(QString::fromStdWString(sql));
    //Bind parameters if binder != nullptr.
    if (result && (binder != nullptr))
        binder->bind(q);
    //Execute query
    if (result)
        result = q.exec();
    //If reader != nullptr iterate through records of query execution result and read each record
    //by calling reader->read(...).
    for (;result && q.next();) {
        result = reader->read(q);
    }
    return result;
}
>

2.5 Gathering all together.

Now with all 3 components mentioned in section 2.1 let's create required function:
<
bool customer(int id, Customer& result)
{
    bool res;
    //Object for reading data described in section 2.2.
    //reader reads data into result.
    Reader2Customer reader(&result);

    //Object for binding parameters described in section 2.3. It binds
    //parameter with name ":id" with id variable.
    QDBBind binder(":id", id);

    //Variable that contains query text.
    wchar_t query[] = L"select * from customer where id = :id";

    //Execute query  and read data into result variable.
    res = db.execSql(query, &reader, &binder);

    return res;
}
>

So having classes <Reader2Customer> and <QDBBind> the result execution of the SQL query from [customer] table with one parameter and returning one record can be read with the call to <execQuery>. In next sections the limitations on parameters number and result records number will be removed.

3. Quering several records from one table.

Let's create function that will read the result of execution of the query [select * from customer where id >= :id_min and id <= :id_max], where [:id_min], [:id_max] are parameters. Unlike the query in section 2.1 this query has 2 parameters and the result of its execution can contain several records. The class <Customer> (see section 2.1) will be used to store customer data. The result of query execution will be stored in the data structure <CustomerVector>, declared as follows:
<
typedef std::vector<Customer> CustomerVector;
>
Here <std::vector> is the container from STL library. So the required function can be declared like this: <bool сustomer(int id_min, int id_max, CustomerVector& с)>.

For reading data into <CustomerVector> dbframework class template <DBReader2STLContainer> will be used. <DBReader2STLContainer> implements data reading into STL-compatible container having push_back method. The method <DBReader2STLContainer.read(...)> reads record using <DBReader2Object> descendant instance and places read object into the container using <push_back>. The template <DBReader2STLContainer> has following parameters:
<Dataset> is a class used to execute the query, in this case it is <QSqlQuery>;
<Object> is a type of the container element, in this case it is <Customer>;
<Container> is a class of the container, in this case it is <CustomerVector>.

So the required class for reading data is <DBReader2STLContainer<QSqlQuery, Customer, CustomerVector> >. Note that dbframework makes it possible to read data not only to STL containers. To achive this it is required to inherit from <DBReader2Container> and implement descendant's method <addToContainer>.

In section 2.3 it was shown how to create class <QDBBind> for binding one parameter. The class template <DBBinders> makes it possible to create a class for binding arbitrary number of parameters by combining instances of <QDBBind> descendants. The class templateа <DBBinders> has one parameter <Dataset> - class used for query execution. So the required method can be implemented in the following way.
<
bool customer(int idmin, int idmax, CustomerVector& v)
{
    bool result = false;

    //Object for reading data into Customer instance (see section 2.2).
    //It is not necessary to link it with Customer instance. It is done by DBReader2Container.
    Reader2Customer rc;

    //The object for reading data into the container. Constructor takes 2 parameters - pointer to CustomerVector
    //and pointer to Reader2Customer. 
    DBReader2STLContainer<QSqlQuery, Customer, CustomerVector> r(&v, &rc);

    //The object for binding parameters. DBBinders has different constructors. This example uses constructor with
    //3 parameters. First two are pointers to objects for parameter binding. The third one has type bool.
    //If it is true then DBReader2Container becomes the owner of passed objects and deallocates them
    //in the destructor.
    DBBinders<QSqlQuery> binder(new QDBBind(":id_min", idmin), new QDBBind(":id_max", idmax), true);

    //The variable containing query text.
    wchar_t query[] = L"select * from customer where id >= :id_min and id <= :id_max";
    
    //Execute query and read data.
    result = db.execSql(query, &r, &binder);    

    return result;
}
>

Now having <Reader2Customer> and using dbframework classes the reading of the results of a query to table [customer] can be reduces to call to <execQuery>.

4. Querying records from joined tables.
 
4.1 Problem statement.

Consider more difficult case of getting data from several joined tables. Let's create function returning data about customer, his accounts and transactions. Such function can be declared as <bool сustomer(int id, CustomerFullInfo& с)>, where <CustomerFullInfo> is a class containing all necessary information.

Following query can get all necessary data:
[
select *
from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account 
where c.id = :id
].
Here [left join] is used to make query return result even for those customers who don't have accounts or transactions.

To create function <customer(...)> it is required to create <CustomerFullInfo> class and a class for reading data.

4.2 Class <CustomerFullInfo>.

It is naturally to create <CustomerFullInfo> using class <Customer> as a base by adding to it a container of objects with information about accounts. The container of objects with transaction data will be added to the class with account info. dbframework makes it possible to use not only containers of object but containers of smart pointers to objects. To demonstrate this feature in this section containers of smart pointers will be used.

Let's create class <CustomerFullInfo>. First classes representing account and transaction  will be created.
<
class Account {
public:
    int id;
    int ref_customer;
};

class Transaction {
public:
    int id;
    int ref_account;
    int amount;
};
>
For now these basic classes don't have containers necessary for representing relations between them. Required containers will be included into 
derivative classes that can be created by inheriting from basic class or by including it as a member. Such approach is more flexible and makes it easy to create required classes for a particular situation.
	
Let's create class <AccountWithTrans> by including  <Account> class and the container for transactions:
<
//Transaction smart pointer.
typedef std::shared_ptr<Transaction> TransactionPtr;
//Transaction container.
typedef std::vector<TransactionPtr> TransactionVector;

//Class representing account and its transactions.
class AccountWithTrans {
public:
    Account account;
    TransactionVector transactions;
};
>

Now it is possible to create class <CustomerFullInfo>. Let's create it by inheriting from <Customer>. But first let's select container for accounts. This selection is a question of balance between the efficiency of reading query results to the container and convenience of using the container in other parts of the program. Consider reading some record from the result of execution of the query from section 4.1. This record contains fields from tables [customer], [account], [tran]. The data from [customer] table is stored into <CustomerFullInfo> fields. Before reading data from [account] table it is required to check if such account has been already added to the account container of <CustomerFullInfo>. This is possible because if there were some operations on the account then the result of query execution contains one record for each transaction. Before reading data from [tran] table it is required to find corresponding account in the account container of <CustomerFullInfo>. So the efficiency of reading depends on the efficiency of the search of the container element by some key. In this case it is naturally to use associative containers such as <map> or <unordered_map> from STL library. For reading to such containers dbframework has class templates <DBReader2STLAssociative> and <DBReader2STLAssociativePtr>. But in other parts of the program it is often more convenient to use sequence containers, such as <vector> from STL library. For such cases dbframework has class template <DBReader2IndexedSTLContainerPtr>. This class uses internal associative container of smart pointers for fast search of elements and in parallel stores read data into linked external container  using <push_back> method. In this example class template <DBReader2IndexedSTLContainerPtr> will be used which makes it possible to use <vector> as a container for accounts.
<
//Account smart pointer.
typedef std::shared_ptr<AccountWithTrans> AccountWithTransPtr;

//Container for accounts.
typedef std::vector<AccountWithTransPtr> AccountWithTransVector;

class CustomerFullInfo : public Customer {
public:
    AccountWithTransVector accounts;
};
>.

4.3 Class for reading data.

Before creating class for reading data into <CustomerFullInfo> let's create classes for reading data into <Account> and <Transaction>. Two important features should be taken into account.

First, the result of execution of the query from section 4.1 contains fields from different tables but with the same name (for example, [id]). The rule for field unique identification is required for correct reading. In this example the rule is defined as following. In the query field names are explicitly defined in the form "table name" - "symbol _" - "field name":
[
select c.id as customer_id, c.name as customer_name, 
a.id as account_id, a.ref_customer as account_ref_customer, 
t.id as tran_id, t.ref_account as tran_ref_account, t.amount as tran_amount
from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account 
where c.id = :id
].
This rule of field naming will be used when implementing classes for data reading.

Second, the result of query execution can contain [NULL] in fields of tables [account], [tran]. To correctly process such cases the implementation of the method <bool read(...)> of classes for data reading will check the correctness of reading and will return corresponding value. In this example the criterion of correct object reading is field [id] being [NOT NULL]. To perform such check let's use the function:
<
bool checkIdField(QString name, QSqlQuery &ds, int* id)
{
    QVariant v = ds.value(name);
    bool result =(v.isValid() && !v.isNull());
    if (result)
        *id = v.toInt();
    return result;
}
>.

Let's modify <Reader2Customer.read(...)> and create classes for reading data into <Account> and <Transaction> taking into account two notes above:
<
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

...

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
>.

To create the class for reading data into <AccountWithTrans> it is required to implement reading into <TransactionVector>. The class template <DBReader2STLContainerPtr> can be used to achieve this. Unlike <DBReader2STLContainer> used in section 3, <DBReader2STLContainerPtr> is designed for reading into the container of smart pointers. This template has following parameters:
<Dataset> - class for query execution, in this case it is <QSqlQuery>;
<Object> - smart pointer to this class is the container element, in this case it is <Transaction>;
<Container> - container class, in this case it is <TransactionVector>;
<ObjectPtr> - smart pointer type, in this case it is <TransactionPtr>.

So the class for reading into <AccountWithTrans> can be created like this:
<
class Reader2AccountWithTrans : public dbframework::DBReader2Object<QSqlQuery, AccountWithTrans> {
private:
    //Object for reading data into Account
    Reader2Account readAccount;
    //Object for reading data into Transaction
    Reader2Transaction readTransaction;    
    //Object for reading data into TransactionVector
    dbframework::DBReader2STLContainerPtr<QSql1Query, Transaction, TransactionVector, TransactionPtr> readTransactions;
public:
    Reader2AccountWithTrans() : DBReader2Object<QSqlQuery, AccountWithTrans>(){};
    Reader2AccountWithTrans(AccountWithTrans* a) : DBReader2Object<QSqlQuery, AccountWithTrans>(a){};
    bool read(QSqlQuery& ds);
};

...

bool Reader2AccountWithTrans::read(QSqlQuery& ds)
{
    //Associate object for reading into Account with AccountWithTrans::account
    readAccount.setObject(&m_object->account);
    //Associate object for reading into TransactionVector with AccountWithTrans::transactions
    readTransactions.setObject(&m_object->transactions);
    //Pass object for reading into Transaction to object for reading into TransactionVector.
    //readTransactions associate readTransaction with correct instance of Transaction.
    readTransactions.setReader(&readTransaction);

    //Correct account information is critical for correct reading.
    bool result = readAccount.read(ds);
    if (result) {
        //It can be no transactions on account. So correctness of reading transactions is not critical.
        readTransactions.read(ds);
    }
    return result;
}
>.
Note that for simplicity in this example associations between classes for reading and storing data are established in the method <read(...)>. It is not efficient because this code is executed at every record reading. It is more efficient to create separate function for this and call it in <Reader2AccountWithTrans(AccountWithTrans* a)> constructor and overridden method <void Reader2AccountWithTrans::setObject(AccountWithTrans* obj)>.

To create the class for reading data into <CustomerFullInfo> it is required to implement reading into <AccountWithTransVector>. As it was mentioned in section 4.2, the template class <DBReader2IndexedSTLContainerPtr> can be used to achive this. This class template has following parameters:
<Dataset> - class for query execution, in this case it is <QSqlQuery>;
<Object> - smart pointer to this class is the container element, in this case it is <AccountWithTrans>;
<Container> - container class, in this case it is <AccountWithTransVector>;
<Key> - type of the unique identifier of <Object> class, in this case it is <int>;
<ObjectPtr> - smart pointer type, in this case it is <AccountWithTransPtr>;
<Index> - associative container class for internal usage, in this case let's use STL library container <map<int, AccountWithTransPtr> >. 

To read data <DBReader2IndexedSTLContainerPtr> requires object instance of the descendant of <DBReader2Object<Dataset, Key> >, that reads unique identifier from query execution result. In this example all object identifiers have type <int> (field <id>), so we'll create single class for reading identifiers:
<
//Reads int value from the field with name defined by QKeyReader::field.
class QKeyReader : public dbframework::DBReader2Object<QSqlQuery, int> {
public:
    QString field;
    QKeyReader() : DBReader2Object<QSqlQuery, int>(){};
    QKeyReader(int* obj) : DBReader2Object<QSqlQuery, int>(obj){};
    bool read(QSqlQuery& ds);
};

...

bool QKeyReader::read(QSqlQuery &ds)
{      
    return checkIdField(field, ds, m_object);
}
>.

Having <QKeyReader> class it is possible to create the class for reading data into <CustomerFullInfo>:
<
//Type of the associative container for internal use in  DBReader2IndexedSTLContainerPtr.
typedef std::map<int, AccountWithTransPtr> AccountWithTransMap;

class Reader2CustomerFullInfo : public dbframework::DBReader2Object<QSqlQuery, CustomerFullInfo> {
private:
    //Object for reading data into Customer
    Reader2Customer readCustomer;
    //Object for reading data into AccountWithTrans
    Reader2AccountWithTrans readAccount;
    //Object for reading unique identifier
    QKeyReader readKey;
    //Object for reading data into AccountWithTransVector
    dbframework::DBReader2IndexedSTLContainerPtr<QSqlQuery, AccountWithTrans, AccountWithTransVector,
        int, AccountWithTransPtr, AccountWithTransMap> readAccounts;
public:
    Reader2CustomerFullInfo() : DBReader2Object<QSqlQuery, CustomerFullInfo>(){};
    Reader2CustomerFullInfo(CustomerFullInfo* c) : DBReader2Object<QSqlQuery, CustomerFullInfo>(c){};
    bool read(QSqlQuery& ds);
};

...

bool Reader2CustomerFullInfo::read(QSqlQuery &ds)
{
    //Associate object fir reading into Customer with CustomerFullInfo
    readCustomer.setObject(m_object);
    //Associate object for reading into AccountWithTransVector with CustomerFullInfo::accounts.
    readAccounts.setObject(&m_object->accounts);
    //Передаем объект для считывания в AccountWithTrans объекту для считывания в AccountWithTransVector. 
    readAccounts.setReader(&readAccount);
    //Associate object for reading unique identifier with the object for reading into AccountWithTransVector. 
    readAccounts.setKeyReader(&readKey);
    readKey.field = "account_id";

    //Correct customer information is critical for correct reading.
    bool result = readCustomer.read(ds);
    if (result) {
        //Customer can have no accounts. So correctness of reading accounts and transactions is not critical.
        readAccounts.read(ds);
    }

    return result;
}
>.

4.4 Class for parameters binding.

It is often convenient to bind query parameters with the fields of some object. It is especially convenient for [insert] or [update] queries. To demonstrate this feature let’s pass the unique identifier to the function <customer> (see section 4.1) in the <id> field of <CustomerFullInfo>. In this case parameter <id> is not required and the function can be declared as <bool сustomer(CustomerFullInfo& с)>.
To create class for parameters binding the class template <DBBindObject> from dbframework can be used. This template has following parameters:
<Dataset> - class for query execution, in this case it is <QSqlQuery>;
<Object> - class which fields are bound with query parameters, in this case it is <Customer>.

Using <DBBindObject> the class for parameters binding can be created like this:
<
class CustomerBinder : public dbframework::DBBindObject<QSqlQuery, Customer> {
public:
    CustomerBinder(Customer* c):DBBindObject<QSqlQuery, Customer>(c){};
    void bind(QSqlQuery& dataset);
};

...

void CustomerBinder::bind(QSqlQuery& dataset)
{
    dataset.bindValue(":customer_id", m_object->id);
    dataset.bindValue(":customer_name", QString::fromStdWString(m_object->name));
}
>.
Note that the query text must be corrected so that еру parameter has name [:customer_id].

4.5 Gathering all together.

Having classes created in sections 4.2 - 4.4 the function <bool сustomer(CustomerFullInfo& с)> can be created as following:
<
bool customer(CustomerFullInfo& c)
{
    bool result = false;
    //Object for reading data into CustomerFullInfo. 
    Reader2CustomerFullInfo rc(&c);

    //Object for binding query parameters and CustomerFullInfo fields. 
    CustomerBinder binder(&c);

    //Variable containing query text.
    wchar_t query[] = L"select c.id as customer_id, c.name as customer_name, \
            a.id as account_id, a.ref_customer as account_ref_customer, \
            t.id as tran_id, t.ref_account as tran_ref_account, t.amount as tran_amount \
            from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account \
            where c.id = :customer_id";

    //Execute query.
    result =  db.execSql(query, &rc, &binder);
    return result;
}
>.
So the use of approaches described in sections 2 - 4 reduces the reading of results of different queries execution to the
creation of new classes for reading data and parameter binding.

5. Files of the example.

example.pro - project file used to build example using Qt SDK.
mybinders.h, mybinders.cpp - parameter binding classes <QDBBind>, <QDBBinders>, <CustomerBinder>.
mydatabase.h, mydatabase.cpp - class <MyDatabase> that implements database creation and filling, query execution.
myreaders.h, myreaders.cpp - classes for data reading <Reader2Customer>, <Reader2Account>, <Reader2Transaction>, <Reader2AccountWithTrans>, <QKeyReader>, <Reader2CustomerFullInfo>.
testtypes.h, testtypes.cpp - classes <Customer>, <Account>, <Transaction>, <AccountWithTrans>, <CustomerFullInfo>.
main.cpp - functions <customer>, function <wmain>.
