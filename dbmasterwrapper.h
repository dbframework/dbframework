#ifndef DBMasterWrapperPtr_H
#define DBMasterWrapperPtr_H

#include "dbdatasetwrapper.h"
#include <map>

/*!
    The DBMasterWrapperAbstract is a class template implementing dataset wrapper that
    reads object from the dataset and adds it to the container of objects. This abstract
    class is a base for creating dataset wrappers for SQL-queries joining tables in
    one to many relation. This tables are refered later as Master and Detail.
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    Object - class that stores data from the Master record.
    ObjectWrapper - object wrapper for class Object.
    Key - class that can be used as unique identifier of Object instance. The Key class  
    is used as a template parameter for assosiative containers and must satisfy the constraints
    imposed by the container.
    AssosiativeContainer - class of the STL assosiative container used to store read data. 
    Container's key value should have type Key. Container's mapped value should have type
    Object or some pointer to Object. Method addObject(...) should be implemented to
    correctly add new object to the container. Example of the AssosiativeContainer:
    std::map<Key, Object>.
    WrapperAssosiativeContainer - class of the STL assosiative container used to store dataset wrappers. 
    Container's key value should have type Key. Container's mapped value should have type
    DBDatasetWrapper<Dataset>::DBDatasetWrapperPtr. Example:
    std::map<Key, DBDatasetWrapper<Dataset>::DBDatasetWrapperPtr>

    Usage scenario.
    The DBMasterWrapperAbstract is used to create dataset wrapper to read data from a dataset 
    formed by SQL-query joining two tables related as one to many.  
    Dataset wrapper for this case is created by inheriting new dataset wrapper class 
    from one of DBMasterWrapperAbstract descendants: DBMasterWrapper (to store Object in AssosiativeContainer)
    or DBMasterWrapperPtr (to store smart pointer to Object in AssosiativeContainer). This classes 
    have properly implemented addObject(...) method. Implement detailWrapper(...) method in the created 
    dataset wrapper class. Use DBDatasetReader<Dataset> descendant to read the data.
    Reading process is described in the read() method description.
    
    For example, let the CMaster class store record from Master, CDetail - from detail. CMasterWrapper and
    CDetailWrapper are their object wrappers. CMaster instances can be identified by integer key. The CMaster has
    a member 
        std::vector<CDetail> details;
    Then dataset wrapper can be created like this.
    
    class CDatasetWrapper : public DBMasterWrapper<Dataset, CMaster, CMasterWrapper, int, 
        std::map<int, CMaster>, std::map<int, DBMasterWrapperAbstract::WrapperPtr> > 
    {
    public:
        CDatasetWrapper(Dataset* ds, Map* storage):DBMasterWrapper(ds, storage){};
    protected:
        DBDatasetWrapper* detailWrapper(Object& obj)
        {
            return new DBDetailWrapper<Dataset, CDetail, CDetailWrapper, std::vector<CDetail> >(dataset(), &obj.details);
        }
    };
    
    By proper implementing detailWrapper(...) it is possible to create dataset wrapper for more complicated queries.
    When Master table is joined with several detail tables, DBDatasetWrapperPair can be used in detailWrapper(...)
    implementation. When Detail table is joined with Detail2 table in one to many relation, then dataset wrapper 
    for the join of Detail and Detail1 can be created first and then used to implement detailWrapper(...).   
*/    
template<class Dataset, class Object, class ObjectWrapper, class Key, 
         class AssosiativeContainer, class WrapperAssosiativeContainer>
class DBMasterWrapperAbstract : public DBDatasetWrapper<Dataset> {
public:    
    /*!
        Sinonym for AssosiativeContainer.
     */
    typedef AssosiativeContainer Map;
    /*!
        Sinonym for WrapperAssosiativeContainer.
     */
    typedef WrapperAssosiativeContainer WrapperMap;
protected:  
    /*!
        Pointer to AssosiativeContainer instance. 
     */
    Map* m_map;
    /*!
        WrapperAssosiativeContainer instance.
     */
    WrapperMap m_wmap;
    /*!
        This method should allocate dataset wrapper, used to read Detail record. Operator new should be
        used for allocation. This dataset wrapper
        will be used to read Detail record every time when Master record represents the passed object.
        It is convinient to use one of the Object members as a container for allocated wrapper. 
        @param[in] obj Object representing Master record.
        @return Pointer to allocated wrapper.
     */
    virtual DBDatasetWrapper* detailWrapper(Object& obj) = 0;
    /*!
        This method should add new Object to the AssosiativeContainer with the
        passed key and return pointer to added Object.
        @param[in] key Key to the Object.
        @return Pointer to added Object.
     */
    virtual Object* addObject(const Key& key) = 0;
public:
    /*!
        Constructor. Should be called in the descendants.
        @param[in] ds Pointer to wrapped dataset.
        @param[in] storage Pointer to the AssosiativeContainer used to store data.
    */
    DBMasterWrapperAbstract(Dataset* ds, Map* storage)
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_map = storage;
    };
    
    virtual ~DBMasterWrapperAbstract();
    /*!
        Reads data from the dataset record.
        Suppose that dataset is produced by SELECT SQL-query joining two tables: Master
        and Detail related as one to many. Each record of the dataset consists of record
        from Master and record from Detail. One to many relation means that dataset may 
        contain many records with the same Master record but different Detail records.        
        While reading from dataset record read() performs following steps.
        1. Retrieve Key of the Master record from dataset by ObjectWrapper::readKey(method).
        2. Check if Master record with Key from step 1 is already present in AssosiativeContainer. 
        3. If step 2 is false, perform 3.1 - 3.3.
            3.1. Add Master record to the AssosiativeContainer with the Key from step 1.
            3.2. Create dataset wrapper for reading Detail record. This instance of the 
            dataset wrapper will be used for reading Detail part of dataset records with
            the same Master part.
            3.3. Store dataset wrapper created in 3.2 in the WrapperAssosiativeContainer
            with the Key from step 1.
        4. Get dataset wrapper from WrapperAssosiativeContainer by the Key from step 1 and
        use it to read Detail record.             
        @return True if following conditions were met:
        - Key successfully read at  step 1;
        - Master record for Key is already inthe AssosiativeContainer or was successfully read.
        Othewise return false. Result of reading of the Detail is ignored.        
     */
    bool read()
    {
        bool result = false;
        Key key;
        result = ObjectWrapper(nullptr).readKey(*dataset(), key);
        if (result) {
            result = false;
            Map::iterator i = m_map->find(key);
            if (i != m_map->end()) {
                if (m_wmap[key].get() !=  nullptr)
                    m_wmap[key]->read();
                result = true;
            }
            else {
                Object *p = addObject(key);
                result = ObjectWrapper(p).read(*dataset());
                m_wmap[key] = DBDatasetWrapperPtr(detailWrapper(*p));
                if (m_wmap[key].get() !=  nullptr)
                    m_wmap[key]->read();                
            }
        }
        return result;
    }
};

/*!
    The DBMasterWrapper is a class template, inherited from DBMasterWrapperAbstract, used when 
    AssosiativeContainer should store Object itself.
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    Object - class that stores data from the Master record.
    ObjectWrapper - object wrapper for class Object.
    Key - class that can be used as unique identifier of Object instance. The Key class  
    is used as a template parameter for assosiative containers and must satisfy the constraints
    imposed by the container.
    AssosiativeContainer - class of the STL assosiative container used to store read data. 
    Container's key value should have type Key. Container's mapped value should have type
    Object. Default value for the AssosiativeContainer is std::map<Key, Object>.
    WrapperAssosiativeContainer - class of the STL assosiative container used to store dataset wrappers. 
    Container's key value should have type Key. Container's mapped value should have type
    DBDatasetWrapper<Dataset>::DBDatasetWrapperPtr. Default value:
    std::map<Key, DBDatasetWrapper<Dataset>::DBDatasetWrapperPtr>
    
     Usage scenario.
     See DBMasterWrapperAbstract description.
*/
template<class Dataset, class Object, class ObjectWrapper, class Key,
         class AssosiativeContainer = std::map<Key, Object>, 
         class WrapperAssosiativeContainer = std::map<Key, DBMasterWrapperAbstract::WrapperPtr> >
class DBMasterWrapper : public DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
        AssosiativeContainer, WrapperAssosiativeContainer > {
public:
    /*!
        Constructor. Should be called in the descendants.
        @param[in] ds Pointer to wrapped dataset.
        @param[in] storage Pointer to the AssosiativeContainer used to store data.
    */
    DBMasterWrapper(Dataset* ds, Map* storage)
        :DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
         AssosiativeContainer, WrapperAssosiativeContainer >(ds, storage)
    {

    };
private:    
    Object* addObject(const Key& key)
    {
        (*m_map)[key] = Object();
        return &((*m_map)[key]);
    }
};

/*!
    The DBMasterWrapperPtr is a class template, inherited from DBMasterWrapperAbstract, used when 
    AssosiativeContainer should store smart pointer to Object.
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    Object - class that stores data from the Master record.
    ObjectWrapper - object wrapper for class Object.
    Key - class that can be used as unique identifier of Object instance. The Key class  
    is used as a template parameter for assosiative containers and must satisfy the constraints
    imposed by the container.
    AssosiativeContainer - class of the STL assosiative container used to store read data. 
    Container's key value should have type Key. Container's mapped value should have type
    std::shared_ptr<Object>. Default value for the AssosiativeContainer is std::map<Key, std::shared_ptr<Object> >.
    WrapperAssosiativeContainer - class of the STL assosiative container used to store dataset wrappers. 
    Container's key value should have type Key. Container's mapped value should have type
    DBDatasetWrapper<Dataset>::DBDatasetWrapperPtr. Default value:
    std::map<Key, DBDatasetWrapper<Dataset>::DBDatasetWrapperPtr>
    
     Usage scenario.
     See DBMasterWrapperAbstract description.
*/
template<class Dataset, class Object, class ObjectWrapper, class Key,
         class AssosiativeContainer = std::map<Key, std::shared_ptr<Object> >, 
         class WrapperAssosiativeContainer = std::map<Key, DBDatasetWrapper<Dataset>::DBDatasetWrapperPtr> >
class DBMasterWrapperPtr : public DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
        AssosiativeContainer, WrapperAssosiativeContainer > {
public:
    typedef std::shared_ptr<Object> ObjectPtr;    
    /*!
        Constructor. Should be called in the descendants.
        @param[in] ds Pointer to wrapped dataset.
        @param[in] storage Pointer to the AssosiativeContainer used to store data.
    */
    DBMasterWrapperPtr(Dataset* ds, Map* storage)
        :DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
            AssosiativeContainer, WrapperAssosiativeContainer >(ds, storage)
    {

    };
private:
    Object* addObject(const Key& key)
    {
        ObjectPtr p(new Object);
        (*m_map)[key] = p;
        return p.get();
    }
};
#endif // DBMasterWrapperPtr_H
