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

#ifndef DBDETAILWRAPPER_H
#define DBDETAILWRAPPER_H

#include "dbdatasetwrapper.h"
#include <memory>
#include <vector>

namespace dbframework {

/*!
    The DBDetailWrapper is a class template implementing dataset wrapper that
    reads object from the dataset and adds it to the container of objects.
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    Object - class that stores data from the dataset record.
    ObjectWrapper - object wrapper for class Object.
    Container - STL container of Object. Container should have method
    insert(iterator position, const Object& val). It can be, for example, std::vector<Object>.    
    
    Usage scenario. 
    The DBDetailWrapper is used to read data from the dataset to the container of Object. 
    The DBDetailWrapper does not clear container before reading. The DBDetailWrapper is 
    suitable in following situations.
    1. When reading from a single table. 
    In this case create instance of DBDetailWrapper with proper template parameters and use 
    DBDatasetReader<Dataset> descendant to read the data.
    2. When reading from a queries, made by joining master table with detail table.
    For example, let query join Master table with Detail table and Master is related with
    Detail as one to many. Class CDetail stores record from Detail table, Class CMaster - 
    from Master table. The CMaster class has field details declared as:
    std::vector<CDetail> details;
    In this case the dataset wrapper for the dataset will be descendant of DBMasterWrapper. 
    DBDetailWrapper can be used to implement DBMasterWrapper::detailWrapper(...) as follows:
    
    DBDatasetWrapper* MasterWrapper::detailWrapper(CMaster& obj) 
    {
        return new DBDetailWrapper<Dataset, CDetail, CDetailWrapper, std::vector<CDetail> >(dataset(), &CMaster.details);
    }
    
    3. When reading from a queries, made by joining tables with one to one relation. For example, 
    let query join Table1 and Table2 tables related as one to one. Class CTable1 stores record
    from Table1, CTable2 - from Table2. Lets declare containers for as follows:
    
    Container<CTable1> c1;
    Container<CTable2> c2;
    
    Then dataset wrapper can be created as follows:
   
    DBDatasetWrapper* w = new DBDatasetWrapperPair<Dataset>(
        new DBDetailWrapper<Dataset, CTable1, CTable1Wrapper, Container<CTable1> >(ds, &c1),
        new DBDetailWrapper<Dataset, CTable2, CTable2Wrapper, Container<CTable2> >(ds, &c2),
        true);
            
    After calling DBDatasetReader<Dataset> descendant read(w), c1 and c2 will be filled with the data from dataset.
    If Container doesn't sort elements internally, CTable1 and CTable2 from the same record will be stored at
    the same position in the containers.
*/
template<class Dataset, class Object, class ObjectWrapper, class Container>
class DBDetailWrapper : public DBDatasetWrapper<Dataset> {
public:
    /*!
        Type of the container.
     */
    typedef Container ObjectContainer;
protected:   
    /*!
        Ponter to the container where to store data.
     */
    ObjectContainer* m_data;
public:
    /*!
        Creates wrapper for dataset.
        @param[in] ds Pointer to dataset. The DBDetailWrapper doesn't take
        ownership of ds.
        @param[in] storage Pointer to container to store data. The DBDetailWrapper 
        doesn't take ownership of ds and doesn't clear it before reading.
     */
    DBDetailWrapper(Dataset *ds, ObjectContainer* storage)
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_data = storage;        
    };        
    /*!
        Reads Object from current record of the dataset using ObjectWrapper::read() and insert
        it and the end of the Container. Note that some container classes can internally sort
        elements.
        @return True if success.
    */
    bool read()
    {
        bool result = false;
        Object p;
        result = ObjectWrapper(&p).read(*DBDatasetWrapper<Dataset>::dataset());        
        if (result)            
            m_data->insert(m_data->end(), p);        
        return result;
    }
};


/*!
    The DBDetailWrapperPtr is a class template implementing dataset wrapper that
    reads object from the dataset and adds it to the container of object smart pointers .
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    Object - class that stores data from the dataset record.
    ObjectWrapper - object wrapper for class Object.
    Container - STL container of std::shared_ptr<Object>. Container should have method
    insert(iterator position, const std::shared_ptr<Object>& val). It can be, for example, 
    std::vector<std::shared_ptr<Object> >.    
    
    Usage scenario. 
    It is the same as for DBDetailWrapper.
*/
template<class Dataset, class Object, class ObjectWrapper, class Container>
class DBDetailWrapperPtr : public DBDatasetWrapper<Dataset> {
public:
    /*!
        Type of smart pointer.
    */
    typedef std::shared_ptr<Object> ObjectPtr;
    /*!
        Type of the container.
    */
    typedef Container ObjectPtrContainer;        
protected:
    /*!
        Ponter to the container where to store data.
     */
    ObjectPtrContainer* m_data;     
public:
    /*!
        Creates wrapper for dataset.
        @param[in] ds Pointer to dataset. The DBDetailWrapper doesn't take
        ownership of ds.
        @param[in] storage Pointer to container to store data. The DBDetailWrapper 
        doesn't take ownership of ds and doesn't clear it before reading.
     */
    DBDetailWrapperPtr(Dataset *ds, ObjectPtrContainer* storage)
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_data = storage;        
    };
    /*!
        Reads Object from current record of the dataset using ObjectWrapper::read() and insert
        it and the end of the Container. Note that some container classes can internally sort
        elements.
        @return True if success.
    */        
    bool read()
    {
        bool result = false;
        ObjectPtr p(new Object);
        result = ObjectWrapper(p.get()).read(*DBDatasetWrapper<Dataset>::dataset());
        if (result)
            m_data->insert(m_data->end(), p);           
        return result;
    }
};

}
#endif // DBDETAILWRAPPER_H
