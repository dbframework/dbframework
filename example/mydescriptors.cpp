#include "mydescriptors.h"

using namespace std;
using namespace dbframework;

MyDescriptor DescriptorCustomer("customer", MyDescriptor::MyContainer({"id"}),MyDescriptor::MyContainer({"name"}));
MyDescriptor DescriptorAccount("account",MyDescriptor::MyContainer({"id"}),MyDescriptor::MyContainer({"ref_customer"}));
MyDescriptor DescriptorTransaction("tran",MyDescriptor::MyContainer({"id"}),MyDescriptor::MyContainer({"ref_account", "amount"}));

MyDescriptor::MyDescriptor(const string& tableName, MyContainer& keyFields, MyContainer& commonFields):
    DBObjectDescriptorImpl<string, size_t, vector<string> >(tableName, keyFields, commonFields)
{

}

string MyDescriptor::parameterName(const string& fieldName) const
{
    return ":" + tableName() + "_" + fieldName;
}
