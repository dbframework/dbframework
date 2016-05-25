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

#ifndef DBDATASETWRAPPER_H
#define DBDATASETWRAPPER_H

#include <memory>

namespace dbframework {

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
    typedef std::shared_ptr<DBDatasetWrapper<Dataset> > DBDatasetWrapperPtr;
};

}

#endif // DBDATASETWRAPPER_H

