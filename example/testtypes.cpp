#include "testtypes.h"

using namespace std;

std::wostream& operator<<(std::wostream& os, const Customer& obj)
{
  os << "Customer(id = " << obj.id << ", name = " << obj.name <<")" << endl;
  return os;
}

std::wostream& operator<<(std::wostream& os, const Account& obj)
{
  os << "  Account(id = " << obj.id << ", ref_customer = " << obj.ref_customer <<")" << endl;
  return os;
}

std::wostream& operator<<(std::wostream& os, const Transaction& obj)
{
    os << "    Transaction(id = " << obj.id << ", ref_account = " << obj.ref_account <<
          ", amount = " << obj.amount << ")" << endl;
    return os;
}

std::wostream& operator<<(std::wostream& os, const AccountWithTrans& obj)
{
    os << obj.account << obj.transactions;
    return os;
}

std::wostream& operator<<(std::wostream& os, const CustomerFullInfo& obj)
{
    os << *((Customer*)(&obj)) << obj.accounts;
    return os;
}

