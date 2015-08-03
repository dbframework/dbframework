#ifndef DBDATASETREADER_H
#define DBDATASETREADER_H

#include "dbdatasetwrapper.h"

/*!
    Descendants of this class reads information from dataset using dataset wrappers.
    Just inherit this class and implement next() method.
*/
template<class Dataset>
class DBDatasetReader
{
public:    
    /*!
        Read data from dataset using object wrapper.
        @param[in] dataset Dataset to read.
        @param[in,out] wrapper Wrapper used to read data.
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
    /*!
        This mehod should move dataset read marker to the next record. If this is
        the first call to next() after retrieving data, it should move read marker 
        to the first record.
        @param[in] ds Pointer to dataset.
        @return True if successfully moved to next record, false if error or
            current record is the last one or dataset empty.
    */
    virtual bool next(Dataset* ds) = 0;
};

#endif // DBDATASETREADER_H
