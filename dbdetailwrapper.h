#ifndef DBDETAILWRAPPER_H
#define DBDETAILWRAPPER_H

#include "dbdatasetwrapper.h"
#include <memory>
#include <vector>

/*!
    Base class for wrappers for detail datasets or simple datasets 
    without relations. 
*/
template<class Dataset, class Object, class ObjectWrapper, class Container>
class DBDetailWrapper : public DBDatasetWrapper<Dataset> {
public:    
    typedef Container ObjectContainer;
protected:    
    ObjectContainer* m_data;
public:
    DBDetailWrapper(Dataset *ds, ObjectContainer* storage)
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_data = storage;        
    };        
    bool read()
    {
        bool result = false;
        Object p;
        result = ObjectWrapper(&p).read(*dataset());        
        if (result)            
            m_data->insert(m_data->end(), p);        
        return result;
    }
};

template<class Dataset, class Object, class ObjectWrapper, class Container>
class DBDetailWrapperPtr : public DBDatasetWrapper<Dataset> {
public:
    typedef std::shared_ptr<Object> ObjectPtr;
    typedef Container ObjectPtrContainer;        
protected:
    ObjectPtrContainer* m_data;     
public:
    DBDetailWrapperPtr(Dataset *ds, ObjectPtrContainer* storage)
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_data = storage;        
    };
            
    bool read()
    {
        bool result = false;
        ObjectPtr p(new Object);
        result = ObjectWrapper(p.get()).read(*dataset());
        if (result)
            m_data->insert(m_data->end(), p);           
        return result;
    }
};


#endif // DBDETAILWRAPPER_H
