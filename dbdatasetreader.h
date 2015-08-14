#ifndef DBDATASETREADER_H
#define DBDATASETREADER_H

#include "dbdatasetwrapper.h"

/*!
    The DBDatasetReader is a class template that implements iteration throw dataset's 
    records and reading data using dataset wrapper. DBDatasetReader is an abstract
    class.
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    
    Usage scenario.
    Inherit from DBDatasetReader providing Dataset template parameter. Implement next(..)
    method. Create class instance and call read(...) to read data from dataset using
    it's wrapper.
*/
template<class Dataset>
class DBDatasetReader
{
public:    
    /*!
        Reads data from dataset using dataset wrapper.
        @param[in] wrapper Dataset wrapper.
        @return True if no errors.
     */
    bool read(DBDatasetWrapper<Dataset>& wrapper)
    {
        bool result = true;       
        for(;next(wrapper.dataset()) && result;) {            
            result = wrapper.read();        
        }
        return result;        
    }
protected:    
    /*!
        This mehod should implement dataset forward navigation. If called for the first time
        after retrieving data to dataset (i.e. after executing SQL query or stored procedure),
        it should navigate to the first record. In this case method should return true if
        dataset is not empty and navigation to first record was succesfull. On next calls
        this method should navigate to the next record. In this case method should return true if
        current record was not the last and successfully moved to next recod. In other words,
        this method should behave like QSqlQuery::next() from Qt library.        
        @param[in] ds Pointer to dataset.
        @return True if success.
    */
    virtual bool next(Dataset* ds) = 0;
};

#endif // DBDATASETREADER_H
