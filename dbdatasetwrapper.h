#ifndef DBDATASETWRAPPER_H
#define DBDATASETWRAPPER_H

#include <memory>

/*!
    The DBDatasetWrapper is a class template that is the ancestor of all 
    dataset wrappers in dbframework. It defines common methods of all
    datset wrappers. Usually it is not required to inherit this class
    directly. It is recommended to inherit some of it's descendants.   
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
*/
template <class Dataset>
class DBDatasetWrapper {
private:
    Dataset* m_dataset;    
public:    
    /*!
        Create wrapper of the dataset. Ownership of the dataset is not taken.
        @param[in] dataset Pointer to dataset to wrap.
    */
    DBDatasetWrapper(Dataset* dataset)
    {
        m_dataset = dataset;
    }
    /*!
        Virtual destructor. Do nothing.
    */
    virtual ~DBDatasetWrapper(){}; 
    /*!
        Get dataset that wrapper wraps.
        @return Pointer to wrapped dataset.
    */
    Dataset* dataset()
    {
        return m_dataset;
    }    
    /*!
        This method should read record from dataset. 
        @return True on successfull read.
    */
    virtual bool read() = 0;    
    typedef std::shared_ptr<DBDatasetWrapper> DBDatasetWrapperPtr;
};



#endif // DBDATASETWRAPPER_H

