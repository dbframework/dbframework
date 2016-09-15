#ifndef TESTTYPES_H
#define TESTTYPES_H

#include <string>
#include <ostream>
#include <memory>
#include <vector>
#include <map>


class Customer {
public:
   int id;
   std::wstring name;
};

typedef std::vector<Customer> CustomerVector;

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

typedef std::shared_ptr<Transaction> TransactionPtr;
typedef std::vector<TransactionPtr> TransactionVector;

class AccountWithTrans {
public:
    Account account;
    TransactionVector transactions;
};

typedef std::shared_ptr<AccountWithTrans> AccountWithTransPtr;
typedef std::map<int, AccountWithTransPtr> AccountWithTransMap;

class CustomerFullInfo : public Customer {
public:
    AccountWithTransMap accounts;
};

std::wostream& operator<<(std::wostream& os, const Customer& obj);
std::wostream& operator<<(std::wostream& os, const Account& obj);
std::wostream& operator<<(std::wostream& os, const Transaction& obj);

template <class T>
std::wostream& operator<<(std::wostream& os, const std::vector<T>& obj)
{
    for (auto i = obj.begin(); i != obj.end(); ++i)
        os << *i;
    return os;
};

template <class T>
std::wostream& operator<<(std::wostream& os, const std::vector<std::shared_ptr<T> >& obj)
{
    for (auto i = obj.begin(); i != obj.end(); ++i)
        os << **i;
    return os;
};

template <class T>
std::wostream& operator<<(std::wostream& os, const std::map<int, std::shared_ptr<T> >& obj)
{
    for (auto i = obj.begin(); i != obj.end(); ++i)
        os << *(i->second);
    return os;
};

std::wostream& operator<<(std::wostream& os, const AccountWithTrans& obj);
std::wostream& operator<<(std::wostream& os, const CustomerFullInfo& obj);
#endif // TESTTYPES_H
