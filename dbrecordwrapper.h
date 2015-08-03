#ifndef DBRECORDWRAPPER_H
#define DBRECORDWRAPPER_H

#include "dbdatasetwrapper.h"

/*!
    Base class for wrappers for detail datasets with one to one relation
    with master dataset. 
*/
template<class Dataset, class Object, class ObjectWrapper>
class DBRecordWrapper : public DBDatasetWrapper<Dataset> {
protected:
    Object* m_data;        
public:
    DBRecordWrapper(Dataset* ds, Object* storage) 
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_data = storage;  
    };

    bool read()
    {
        return ObjectWrapper(m_data).read(*dataset());
    }
};

#endif // DBRECORDWRAPPER_H
