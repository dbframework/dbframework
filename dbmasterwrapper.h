#ifndef DBMasterWrapperPtr_H
#define DBMasterWrapperPtr_H

#include "dbdatasetwrapper.h"
#include <map>
#include <memory>
#include <vector>

template<class Dataset, class Object, class ObjectWrapper, class Key, class AssosiativeContainer>
class DBMasterWrapperAbstract : public DBDatasetWrapper<Dataset> {
public:
    typedef AssosiativeContainer Map;
protected:
    typedef std::shared_ptr<DBDatasetWrapper> WrapperPtr;
    typedef std::map<Key, WrapperPtr> WrapperMap;
    Map* m_map;
    WrapperMap m_wmap;
    /*!
        This method should allocate wrapper to the provided object.
        @param[in] obj Object to allocater wrapper for.
        @return Wrapper pointer.
     */
    virtual DBDatasetWrapper* detailWrapper(Object& obj) = 0;
    virtual Object* insertObject(const Key& key) = 0;
private:
    void readObject(Object* obj)
    {
        ObjectWrapper(obj).read(*dataset());
    }
public:
    DBMasterWrapperAbstract(Dataset* ds, Map* storage)
        :DBDatasetWrapper<Dataset>(ds)
    {
        m_map = storage;
    };

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
                Object *p = insertObject(key);
                readObject(p);
                m_wmap[key] = WrapperPtr(detailWrapper(*p));
                if (m_wmap[key].get() !=  nullptr)
                    m_wmap[key]->read();
                result = true;
            }
        }
        return result;
    }
};


/*!
    Base class for wrappers for datasets with master-detail relation. 
*/
template<class Dataset, class Object, class ObjectWrapper, class Key>
class DBMasterWrapperPtr : public DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
        std::map<Key, std::shared_ptr<Object> > > {
public:
    typedef std::shared_ptr<Object> ObjectPtr;    
    DBMasterWrapperPtr(Dataset* ds, Map* storage)
        :DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
            std::map<Key, std::shared_ptr<Object> > >(ds, storage)
    {

    };
private:
    Object* insertObject(const Key& key)
    {
        ObjectPtr p(new Object);
        (*m_map)[key] = p;
        return p.get();
    }
};

template<class Dataset, class Object, class ObjectWrapper, class Key>
class DBMasterWrapper : public DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
        std::map<Key, Object> > {
public:
    DBMasterWrapper(Dataset* ds, Map* storage)
        :DBMasterWrapperAbstract<Dataset, Object, ObjectWrapper, Key,
         std::map<Key, Object> >(ds, storage)
    {

    };
private:
    Object* insertObject(const Key& key)
    {
        (*m_map)[key] = Object();
        return &((*m_map)[key]);
    }
};

#endif // DBMasterWrapperPtr_H
