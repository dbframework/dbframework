#ifndef MYDESCRIPTORS_H
#define MYDESCRIPTORS_H

#include "dbframework.h"
#include <vector>
#include <string>

class MyDescriptor : public dbframework::DBObjectDescriptorImpl<std::string, size_t, std::vector<std::string> >
{
public:
    typedef std::vector<std::string> MyContainer;
    MyDescriptor(const std::string& tableName, MyContainer& keyFields, MyContainer& commonFields);
    std::string parameterName(const std::string& fieldName) const;
};

extern MyDescriptor DescriptorCustomer;
extern MyDescriptor DescriptorAccount;
extern MyDescriptor DescriptorTransaction;

#endif // MYDESCRIPTORS_H
