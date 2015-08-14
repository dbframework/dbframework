#ifndef DBRECORDWRAPPER_H
#define DBRECORDWRAPPER_H

#include "dbdatasetwrapper.h"

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

DBDatasetWrapper* MasterWrapper::detailWrapper(CMaster& obj) 
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
        return ObjectWrapper(m_data).read(*dataset());
    }
};

#endif // DBRECORDWRAPPER_H
