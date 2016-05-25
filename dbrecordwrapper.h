/*
Copyright (c) 2015 Sidorov Dmitry

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  
*/ 

#ifndef DBRECORDWRAPPER_H
#define DBRECORDWRAPPER_H

#include "dbdatasetwrapper.h"

namespace dbframework {

/*!
The DBRecordWrapper is a class template implementing dataset wrapper that
reads record and stores it in the object.

Template parameters.
Dataset - class implementing functionality of presenting data, retrieved by SQL
query or stored procedure call, as a container of records supporting forward record
iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
Object - class that stores data from the dataset record.
ObjectWrapper - object wrapper for class Object.

Usage scenario. 
The DBRecordWrapper is used to read data from the dataset to the Object. The DBDetailWrapper is 
suitable in following situations.
1. When reading from a SQL-query tha returns one record. 
In this case create instance of DBDetailWrapper with proper template parameters and use 
DBDatasetReader<Dataset> descendant to read the data.
2. When reading from a queries, made by joining master table with detail table related as 
one to one. For example, let query join Master table with Detail table and Master is related with
Detail as one to one. Class CDetail stores record from Detail table, Class CMaster - 
from Master table. The CMaster class has field:
CDetail detail;
In this case the dataset wrapper for the dataset will be descendant of DBMasterWrapper. 
DBRecordWrapper can be used to implement DBMasterWrapper::detailWrapper(...) as follows:

dbframework::DBDatasetWrapper* MasterWrapper::detailWrapper(CMaster& obj) 
{
    return new DBRecordWrapper<Dataset, CDetail, CDetailWrapper>(dataset(), &detail);
}
*/
template<class Dataset, class Object, class ObjectWrapper>
class DBRecordWrapper : public DBDatasetWrapper<Dataset> {
protected:
    /*!
        Pointer to Object instance used to store data.
     */
    Object* m_data;        
public:
    /*!
        Create DBRecordWrapper instance.
        @param[in] ds Pointer to wrapped dataset.
        @param[in] storage Pointer to Object instance used to store data. 
     */
    DBRecordWrapper(Dataset* ds, Object* storage) 
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_data = storage;  
    };
    /*!
        Read record from the dataset using ObjectWrapper and store it in the Object.
        @return True if successfully read.
     */
    bool read()
    {
        return ObjectWrapper(m_data).read(*DBDatasetWrapper<Dataset>::dataset());
    }
};

}
#endif // DBRECORDWRAPPER_H
