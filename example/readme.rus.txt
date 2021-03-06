﻿0. Введение.

Данное приложение представляет собой пример использования dbframework для создания приложения, осуществляющего выполнение запросов к базе данных. В качстве базы данных используется SQLite. Для компиляции примера потребуется Qt SDK версии не ниже 5.2.0. Файл проекта example.pro.

В результате компиляции создается консольное приложение. При запуске приложению передается один параметр - имя файла базы данных. Приложение создает базу данных, заполняет ее и выводит на экран результат выполнения 3 запросов к БД. 

В следующих разделах подробно рассматривается создание данного приложения. При этом используются следующие обозначения. В квадратные скобки [] заключаются названия объектов базы данных (названия таблиц, полей и т.д.) и тексты SQL-запросов. В угловые скобки заключаются синтаксические элементы C++ (названия типов, переменных и т.д.) и фрагменты исходных текстов.

1. Структура базы данных примера.

В данном примере используется база данных SQLite о клиентах, их счетах и операциях по счетам. База данных содержит 3 таблицы: [customer] (информация о клиентах),  [account] (информация о счетах), [tran] (информация об операциях). Таблица [customer] связана с таблицей [account] отношением один-ко-многим. Таблица [account] связана c таблицей [tran] отношением один-ко-многим.

Таблица [customer] содержит информацию о клиентах. Поля таблицы:
[id] - уникальный идентификатор клиента;
[name] - имя клиента.

Таблица [account] содержит информацию о счетах клиентов. Поля таблицы:
[id] - уникальный номер счета;
[ref_customer] - идентификатор клиента, владеющего счетом, в таблице [customer].

Таблица [tran] содержит информацию об операциях по счетам. Поля таблицы:
[id] - уникальный идентификатор операции;
[ref_account] - идентификатор счета в таблице [account];
[amount] - сумма операции.

2. Запрос одной записи из одной таблицы.

2.1 Постановка задачи.

Создадим функцию, которая будет получать из базы данных результат выполнения запроса
[select * from customer where id = :id], где [:id] - параметр. 

Данные о клиентах будем представлять с помощью класса <Customer>:
<
class Customer {
public:
   int id;
   std::wstring name;
};
>.
В примере для простоты поля класса объявлены как public. Ничто не мешает объявить их как private и использовать методы для доступа кним. Требуемая функция будет иметь вид <bool сustomer(int id, Customer& с)>.

Результат выполнения запроса к базе данных является набором записей. Для считывания данных с помощью dbframework нужно 3 компонента:
- класс, ответственный за считывание данных из одной записи в требуемую структуру данных;
- класс, ответственный за передачу значений параметров запроса;
- функция, ответственная за управление выполнением запроса и считыванием данных с помощью с помощью этих классов.

2.2 Класс для считывания данных.

Класс для считывания данных должен быть потомком класса <DBReader>. В рассматриваемом случае считывание происходит в экземпляр класса <Customer>, поэтому унаследуем создаваемый класс от <DBReader2Object>. <DBReader2Object> - шаблон класса с параметрами <Dataset> и <Object>. <Dataset> - класс, используемый для выполнения запроса. В данном примере для выполнения запросов к базе данных используется библиотека Qt, поэтому в качестве <Dataset> используется класс <QSqlQuery> из библиотеки Qt. <Object> - это класс, в экземпляр которого происходит считывание данных. Назовем создаваемый класс <Reader2Customer>:
<
class Reader2Customer : public dbframework::DBReader2Object<QSqlQuery, Customer> {
public:
    Reader2Customer() : DBReader2Object<QSqlQuery, Customer>(){};
    Reader2Customer(Customer* c) : DBReader2Object<QSqlQuery, Customer>(c){};
    bool read(QSqlQuery& ds);
};
>
В <Reader2Customer> необходимо реализовать абстрактный метод <bool read(QSqlQuery& ds)>. Сделаем это следующим образом:
<
bool Reader2Customer::read(QSqlQuery& ds)
{
    //считываем поле id 
    m_object->id = ds.value("id").toInt();
    //считываем поле name
    m_object->name = ds.value("name").toString().toStdWString();    
    return true;
}
>.
В данной упрощенной реализации не осуществляется контроль правильности считывания.

2.3 Класс для передачи значений параметров.

Класс для передачи значений параметров должен быть потомком класса <DBBinder>. В рассматриваемом случае требуется передача одного параметра, поэтому унаследуем создаваемый класс от <DBBind>. <DBBind> - шаблон класса с параметрами  <Dataset>, <NameType>, <ValueType>. <DBBind> предназначен для реализации класса передающего значение параметра по его имени. <Dataset> - класс, используемый для выполнения запроса. В рассматриваемом случае - <QSqlQuery>. <NameType> - тип, используемый для задания имени параметра. Будем использовать <QString> - динамическая строка из библиотеки Qt. <ValueType> - тип, используемый для задания значения параметра. Будем использовать класс <QVariant> - класс, способный хранить данные разных типов из библиотеки Qt. Назовем создаваемый класс <QDBBind>:
<
class QDBBind : public dbframework::DBBind<QSqlQuery, QString, QVariant> {
public:
    QDBBind(const QString& name, const QVariant& value):DBBind<QSqlQuery, QString, QVariant>(name, value){};
    void bind(QSqlQuery& dataset);
};
>
В <QDBBind> необходимо реализовать абстрактный метод <void bind(QSqlQuery& ds)>. Сделаем это следующим образом:
<
void QDBBind::bind(QSqlQuery& dataset)
{
    //m_name, m_value - protected поля, унаследованные от DBBind. Их значение задается в конструкторе.
    //Устанавливаем параметру m_name значение m_value.
    dataset.bindValue(m_name, m_value);
}
>.

2.4 Функция управления считыванием данных.

В данном примере функция управления считыванием данных реализована в виде метода класса <MyDatabase>:
<
/*
    sql - текст SQL-запроса.
    reader - экземпляр класса для считывания данных.
    binder - экземпляр класса для передачи значений параметров.
*/
bool MyDatabase::execSql(wchar_t *sql, DBReader<QSqlQuery>* reader, DBBinder<QSqlQuery>* binder)
{
    QSqlQuery q(db);
    bool result = q.prepare(QString::fromStdWString(sql));

    if (result)
        //Для выполнения запроса используем потомка DBSQLExecutor 
        result = MySQLExec().exec(q, binder, reader);

    return result;    
}
>

2.5 Собираем все вместе.

Теперь, имея все 3 компонента, перечисленные в  п. 2.1, создадим функцию требуемую функцию:
<
bool customer(int id, Customer& result)
{
    bool res;
    //Объект для считывания данных из результата запроса в класс Customer, описанный в п. 2.2.
    //Объект <reader>  осуществляет считывание в объект <result>.
    Reader2Customer reader(&result);

    //Объект для передачи параметров запроса, рассмотренный в п. 2.3. Данный объект устанавливает
    //значение праметра c именем ":id" равным значению переменной <id>.
    QDBBind binder(":id", id);

    //Переменная, содержащая текст запроса.
    wchar_t query[] = L"select * from customer where id = :id";

    //Выполнение запроса к базе данных.
    res = db.execSql(query, &reader, &binder);

    return res;
}
>

Таким образом, имея классы <Reader2Customer> и <QDBBind>, результат запроса с одним  параметром к таблице [customer], возвращающий одно значение, может быть считан вызовом функции <execQuery>. В следующих разделах примера будут сняты ограничения на количество параметров и записей в результате запроса.

3. Запрос нескольких записей из одной таблицы. 

Создадим функцию, которая будет получать из базы данных результат выполнения запроса [select * from customer where id >= :id_min and id <= :id_max], где [:id_min], [:id_max] - параметры. В отличии от запроса, рассмотренного в п. 2.1 данный запрос имеет 2 параметра и результат его выполнения может содержать несколько записей. Для представления данных о клиенте воспользуемся ранее созданным классом <Customer> (см. п. 2.1). Результат выполнения запроса будем сохранять в cтруктуре данных  CustomerVector>, объявленной следующим образом:
<
typedef std::vector<Customer> CustomerVector;
>
Здесь <std::vector> - контейнер из библиотеки STL. Таким образом, требуемая функция будет иметь вид <bool сustomer(int id_min, int id_max, CustomerVector& с)>.

Для считывания данных в <CustomerVector> используем шаблон класса dbframework <DBReader2STLContainer>. <DBReader2STLContainer> позволяет организовать считывание данных в STL-совместимый контейнер, имеющий метод push_back. Метод <DBReader2STLContainer.read(...)> считывает запись с помощью экземпляра потомка <DBReader2Object> и помещает считанный объект в контейнер с помощью метода <push_back>. Шаблон <DBReader2STLContainer> имеет следующие параметры:
<Dataset> - класс, используемый для выполнения запроса, в рассматриваемом случае это <QSqlQuery>;
<Object> - класс элемента контайнера, в рассматриваемом случае это <Customer>;
<Container> - класс контейнера, в в рассматриваемом случае это <CustomerVector>.

Таким образом, требуемый класс для считывания это <DBReader2STLContainer<QSqlQuery, Customer, CustomerVector> >. Отметим, что dbframework позволяет организовать считывание данных не только в контейнеры STL. Для этого нужно унаследовать класс от <DBReader2Container> и реализовать метод <addToContainer>.

В п. 2.3 было показано, как создать класс <QDBBind> для передачи одного параметра. Шаблон класса <DBBinders> позволяет создать класс для передачи любого числа параметров путем комбинирования экземпляров класса <QDBBind>. Шаблон класса <DBBinders> имеет один параметр <Dataset> - класс, используемый для выполнения запроса. Таким образом, требуемый метод может быть реализован следующим образом.
<
bool customer(int idmin, int idmax, CustomerVector& v)
{
    bool result = false;

    //Объект для считывания данных в объект Customer (см. п. 2.2).
    //Связывать его с экземпляром Customer не нужно. Это делает DBReader2Container.
    Reader2Customer rc;

    //Объект для считывания данных в контейнер. Крнструктор принимает два параметра - указатель на CustomerVector
    //и указатель на Reader2Customer. 
    DBReader2STLContainer<QSqlQuery, Customer, CustomerVector> r(&v, &rc);

    //Объект для передачи параметров. DBBinders имеет несколько конструкторов. В данном примере используется конструктор
    //c 3 параметрами. Первые два - указатели на объекты для передачи параметров. Третий параметр имеет тип bool.
    //Если он имеет значение true, то DBReader2Container становится владельцем переданных ему объектов, т.е. освобождает
    //их при вызове деструктора.
    DBBinders<QSqlQuery> binder(new QDBBind(":id_min", idmin), new QDBBind(":id_max", idmax), true);

    //Переменная, содержащая текст запроса.
    wchar_t query[] = L"select * from customer where id >= :id_min and id <= :id_max";
    
    //Выполнение запроса к базе данных.
    result = db.execSql(query, &r, &binder);    

    return result;
}
>

Теперь располагая классом <Reader2Customer> и используя классы dbframework можно свести считывание результатов любого запроса к таблице [customer] к вызову метода <execQuery>.

4. Запрос записей из связанных таблиц. 
 
4.1 Постановка задачи.

Рассмотрим теперь более сложный случай - получение данных из нескольких связанных таблиц. Предположим, нужно создать функцию, возвращающую данные о клиенте, всех его счетах и операциях по ним. Такая функция может иметь вид <bool сustomer(int id, CustomerFullInfo& с)>, где <CustomerFullInfo> - некоторый класс, содержащий всю нужную информацию.

Получить всю необходимую информацию можно с помощью следующего запроса:
[
select *
from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account 
where c.id = :id
].
Здесь используется конструкция [left join], чтобы запрос возвращал результат даже для тех клиентов, у которых нет счетов или операций по счету.

Для создания функции <customer(...)> нужно определить тип <CustomerFullInfo> и создать класс для считывания данных.

4.2 Класс <CustomerFullInfo>.

Класс <CustomerFullInfo> естественно создать на базе класса <Customer> и добавить в него контейнер для объектов, содержащих информацию о счетах. В класс, содержащий информацию о счете, включим контейнер, содержащий объекты, описывающие операцию. dbframework позволяет использовать не только контейнеры, содержащие объекты, но и контейнеры умных указателей на объекты. Для демонстрации этой возможности в данном разделе будем использовать контейнеры умных указателей.

Приступим к созданию класса <CustomerFullInfo>. Сначала создадим классы для представления счета и операции как показано ниже.
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
Эти базовые классы пока не содержат контейнеров, необходимых для отражения связей между ними. Необходимые контейнеры включим в производные классы, которые могут быть созданы путем наследования от базового класса или включения базового класса. Такой подход более гибок и позволяет легко порождать группы классов, необходимых в конкретной ситуации. 

Создадим класс <AccountWithTrans> путем включения в него классов <Account> и контейнера операций:
<
//Умный указатель на операцию.
typedef std::shared_ptr<Transaction> TransactionPtr;
//Контейнер операций.
typedef std::vector<TransactionPtr> TransactionVector;

//Класс, содержащий информацию о счете и операциях.
class AccountWithTrans {
public:
    Account account;
    TransactionVector transactions;
};
>

Теперь можно создать класс <CustomerFullInfo>. Создадим его путем наследования от <Customer>. Однако сначала выберем контейнер для счетов. Этот выбор является вопросом баланса между эффективностью считывания результатов запроса в данный контейнер и удобством использования его в остальных частях программы. Рассмотрим процесс считывания  некоторой записи результата выполнения запроса из п. 4.1. Данная запись содержит поля из таблиц [customer], [account], [tran]. Данные из таблицы [customer] заносятся в поля <CustomerFullInfo>. Перед чтением данных из таблицы [account] нужно предварительно проверить, не был ли уже такой счет добавлен в контейнер счетов <CustomerFullInfo>. Такое возможно, т.к. если по счету выполнено несколько операций, то результат выполнения запроса будет содержать по одной записи для каждой операции. Перед чтением данных из таблицы [tran] нужно найти в контейнере счетов <CustomerFullInfo> соответствующий счет. Таким образом, на эффективность процедуры считывания влияет скорость поиска элемента в контейнере по некоторому ключевому значению. В такой ситуации естественным является использование ассоциативных контейнеров, таких как <map> или <unordered_map> из библиотеки STL. Для считывания в такие контейнеры в dbframework есть шаблоны классов <DBReader2STLAssociative> и <DBReader2STLAssociativePtr>. Однако в остальных частях программы часто удобнее использовать контейнер с последовательным расположением элементов, такой как <vector> из библиотеки STL. Для таких случаев dbframework содержит шаблон класса <DBReader2IndexedSTLContainerPtr>. Этот класс использует внутренний ассоциативный контейнер умных указателей для быстрого поиска элементов и параллельно помещает считанные данные в связанный с ним внешний контейнер с помощью метода <push_back>. В данном примере будет использован шаблон класса <DBReader2IndexedSTLContainerPtr>, что позволяет в качестве контейнера для счетов использовать <vector>.
<
//Умный указатель на счет.
typedef std::shared_ptr<AccountWithTrans> AccountWithTransPtr;

//Контейнер для счетов
typedef std::vector<AccountWithTransPtr> AccountWithTransVector;

class CustomerFullInfo : public Customer {
public:
    AccountWithTransVector accounts;
};
>.

4.3 Класс для считывания данных.

Для создания класса для считывания данных в <CustomerFullInfo> предварительно создадим классы для считывания данных в класcы <Account> и <Transaction>. При этом нужно учесть два существенных момента.

Во-первых, результат выполнения запроса из п. 4.1 содержит поля из разных таблиц с одинаковыми именами (например [id]). Для корректного считывания необходимо некоторое правило для однозначной идентификации полей. В данном примере поступим следующим образом. В запросе в явном виде зададим имена полей в формате "имя таблицы" - "знак _" - "имя поля":
[
select c.id as customer_id, c.name as customer_name, 
a.id as account_id, a.ref_customer as account_ref_customer, 
t.id as tran_id, t.ref_account as tran_ref_account, t.amount as tran_amount
from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account 
where c.id = :id
].
При реализации классов для считывания данных будем использовать введенное правило именования полей.

Во-вторых, результат выполнения запроса может содержать [NULL] в полях таблиц [account], [tran]. Для корректной обработки этих случаев при реализации метода <bool read(...)> классов для считывания данных проверять корректность считывания и возвращать соответствующее значение. В данном примере критерием корректности считывания объекта будем считать значение [NOT NULL] поля [id]. Для выполнения такой проверки воспользуемся следующей функцией:
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

С учетом двух приведенных выше замечаний модифицируем <Reader2Customer.read(...)> и создадим классы для считывания данных в <Account> и <Transaction>:
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

При создании класса для считывания данных в <AccountWithTrans> понадобится реализовать считывание в <TransactionVector>. Для этого можно воспользоваться шаблоном класса <DBReader2STLContainerPtr> из dbframework. В отличии от <DBReader2STLContainer>, использовавшегося в п.3, <DBReader2STLContainerPtr> предназначен для считывания в контейнер умных указателей. Данный шаблон имеет следующие параметры:
<Dataset> - класс, используемый для выполнения запроса, в рассматриваемом случае это <QSqlQuery>;
<Object> - класс, умный указатель на который является элементом контейнера, в рассматриваемом случае это <Transaction>;
<Container> - тип контейнера, в рассматриваемом случае это <TransactionVector>;
<ObjectPtr> - тип умного указателя, в рассматриваемом случае это <TransactionPtr>.

Итак, создадим класс для считывания в <AccountWithTrans> следующим образом:
<
class Reader2AccountWithTrans : public dbframework::DBReader2Object<QSqlQuery, AccountWithTrans> {
private:
    //Объект для считывания данных в Account
    Reader2Account readAccount;
    //Объект для считывания данных в Transaction
    Reader2Transaction readTransaction;    
    //Объект для считывания данных в TransactionVector
    dbframework::DBReader2STLContainerPtr<QSql1Query, Transaction, TransactionVector, TransactionPtr> readTransactions;
public:
    Reader2AccountWithTrans() : DBReader2Object<QSqlQuery, AccountWithTrans>(){};
    Reader2AccountWithTrans(AccountWithTrans* a) : DBReader2Object<QSqlQuery, AccountWithTrans>(a){};
    bool read(QSqlQuery& ds);
};

...

bool Reader2AccountWithTrans::read(QSqlQuery& ds)
{
    //Связываем объект для считывания в Account с AccountWithTrans::account
    readAccount.setObject(&m_object->account);
    //Связываем объект для считывания в TransactionVector с AccountWithTrans::transactions
    readTransactions.setObject(&m_object->transactions);
    //Передаем объект для считывания в Transaction объекту для считывания в TransactionVector.
    //readTransactions сам связывает readTransaction с нужным экземпляром Transaction.
    readTransactions.setReader(&readTransaction);

    //Правильность считывания информации о счете необходима для признания считывания корректным.
    bool result = readAccount.read(ds);
    if (result) {
        //По счету может не быть операций. Поэтому правильность считывания транзакций не критична.
        readTransactions.read(ds);
    }
    return result;
}
>.
Отметим, что в данном примере для простоты взаимосвязи между классами для чтения и хранения данных устанавливаются в методе <read(...)>. Это не эффективно, т.к. данный код выполняется при чтении каждой записи результата выполнения запроса. Правильнее было бы создать для связывания отдельную функцию и вызывать ее в конструкторе <Reader2AccountWithTrans(AccountWithTrans* a)>, а также в переопределенном методе <void Reader2AccountWithTrans::setObject(AccountWithTrans* obj)>.

При создании класса для считывания данных в <CustomerFullInfo> понадобится реализовать считывание в <AccountWithTransVector>. Как уже было отмечено в п. 4.2, для этого можно воспользоваться шаблоном класса <DBReader2IndexedSTLContainerPtr>. Данный шаблон имеет следующие параметры:
<Dataset> - класс, используемый для выполнения запроса, в рассматриваемом случае это <QSqlQuery>;
<Object> - класс, умный указатель на который является элементом контейнера, в рассматриваемом случае это <AccountWithTrans>;
<Container> - тип контейнера, в рассматриваемом случае это <AccountWithTransVector>;
<Key> - тип уникального идентификатора объектов класса <Object>, в рассматриваемом случае это <int>;
<ObjectPtr> - тип умного указателя, в рассматриваемом случае это <AccountWithTransPtr>;
<Index> - класс ассоциативного контейнера для внутреннего использования, в рассматриваемом случае будем использовать  контейнер <map<int, AccountWithTransPtr> > из библиотеки STL. 

Для считывания данных <DBReader2IndexedSTLContainerPtr> требуется объект, порожденный от <DBReader2Object<Dataset, Key> >, осуществляющий считывание уникального идентификатора из результата выполнения запроса. Поскольку в нашем примере все идентификаторы объектов имеют тип <int> (поле <id>), создадим один класс для считывания идентификаторов:
<
//Считывает значение типа int из поля, имя которого задано в QKeyReader::field.
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

Имея класс <QKeyReader>, можно создать класс для считывания данных в <CustomerFullInfo>:
<
//Тип ассоциативного контейнера для внутреннего использования в DBReader2IndexedSTLContainerPtr.
typedef std::map<int, AccountWithTransPtr> AccountWithTransMap;

class Reader2CustomerFullInfo : public dbframework::DBReader2Object<QSqlQuery, CustomerFullInfo> {
private:
    //Объект для считывания данных в Customer
    Reader2Customer readCustomer;
    //Объект для считывания данных в AccountWithTrans
    Reader2AccountWithTrans readAccount;
    //Объект для считывания уникального идентификатора
    QKeyReader readKey;
    //Объект для считывания данных в AccountWithTransVector
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
    //Связываем объект для считывания в Customer с CustomerFullInfo
    readCustomer.setObject(m_object);
    //Связываем объект для считывания в AccountWithTrans с CustomerFullInfo::accounts
    readAccounts.setObject(&m_object->accounts);
    //Передаем объект для считывания в AccountWithTrans объекту для считывания в AccountWithTransVector. 
    readAccounts.setReader(&readAccount);
    //Передаем объект для считывания уникального идентификатора объекту для считывания в AccountWithTransVector. 
    readAccounts.setKeyReader(&readKey);
    readKey.field = "account_id";

    //Правильность считывания информации о клиенте необходима для признания считывания корректным.
    bool result = readCustomer.read(ds);
    if (result) {
        //Клиент может не иметь счетов. Поэтому правильность считывания счетов не критична.
        readAccounts.read(ds);
    }

    return result;
}
>.

4.4 Класс для передачи параметров.

При передаче параметров запроса часто бывает удобно передать параметры в полях некоторого объекта. Особенно это удобно в запросах [insert] и [update]. Чтобы продемонстировать такую возможность, передадим в функцию <customer> (см. п. 4.1) уникальный идентификатор в поле <id> класса <CustomerFullInfo>. В этом случае необходимость в параметре <id> отпадает и создаваемая функция будет иметь вид <bool сustomer(CustomerFullInfo& с)>.
Для создания класса для передачи параметров воспользуемся шаблоном класса <DBBindObject> из dbframework. Этот шаблон имеет следующие параметры:
<Dataset> - класс, используемый для выполнения запроса, в рассматриваемом случае это <QSqlQuery>;
<Object> - класс, в полях которого передаются параметры, будем использовать <Customer>.

Используя <DBBindObject> создадим класс для передачи параметров следующим образом:
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
При этом нужно исправить текс запроса так, чтобы параметр имел имя [:customer_id].

4.5 Собираем все вместе.

Имея классы, созданные в п. 4.2 - 4.4, функцию <bool сustomer(CustomerFullInfo& с)> можно создать следующим образом:
<
bool customer(CustomerFullInfo& c)
{
    bool result = false;
    //Объект для считывания данных в CustomerFullInfo. 
    Reader2CustomerFullInfo rc(&c);

    //Объект для передачи параметров в полях CustomerFullInfo. 
    CustomerBinder binder(&c);

    //Переменная, содержащая текст запроса.
    wchar_t query[] = L"select c.id as customer_id, c.name as customer_name, \
            a.id as account_id, a.ref_customer as account_ref_customer, \
            t.id as tran_id, t.ref_account as tran_ref_account, t.amount as tran_amount \
            from (customer c left join account a on c.id = a.ref_customer) left join tran t on a.id = t.ref_account \
            where c.id = :customer_id";

    //Выполнение запроса к базе данных.
    result =  db.execSql(query, &rc, &binder);
    return result;
}
>.
Таким образом, используя приемы, описанные в п. 2 - 4, считывание результатов выполнения различных запросов 
сводится к созданию новых классов для считывания данных и, возможно, для передачи параметров.

5. Файлы, входящие в состав примера.

example.pro - файл проекта для компиляции примера с помощью Qt SDK.
mybinders.h, mybinders.cpp - классы для передачи параметров <QDBBind>, <QDBBinders>, <CustomerBinder>.
mydatabase.h, mydatabase.cpp - класс <MyDatabase>, осуществляющий создание и заполнение БД, выполнение запросов.
myreaders.h, myreaders.cpp - клаccы для чтения данных <Reader2Customer>, <Reader2Account>, <Reader2Transaction>, <Reader2AccountWithTrans>, <QKeyReader>, <Reader2CustomerFullInfo>.
mydescriptors.h, mydescriptors.cpp - класс <MyDescriptor> для описания структуры таблицы в БД.
testtypes.h, testtypes.cpp - классы <Customer>, <Account>, <Transaction>, <AccountWithTrans>, <CustomerFullInfo>.
main.cpp - функции <customer>, функция <wmain>.