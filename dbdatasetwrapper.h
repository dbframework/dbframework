#ifndef DBDATASETWRAPPER_H
#define DBDATASETWRAPPER_H

/*!
    Base class for implementing dataset wrapper. Dataset wrapper is
    a class which helps to read data from the dataset to some storage
    using DBDatasetReader. Storage can be object, container of objects etc.
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
        @return Return true on successfull read.
    */
    virtual bool read() = 0;    
};

#endif // DBDATASETWRAPPER_H
