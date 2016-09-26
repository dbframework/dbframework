#ifndef DBREADER2STLASSOCIATIVEPTR_H
#define DBREADER2STLASSOCIATIVEPTR_H

#include "dbreader2associative.h"

namespace dbframework {

/*!
    The DBReader2STLAssociativePtr template class is the DBReader2Associative descendant that supports STL associative
    containers map and unordered_map and other compatible containers. Compatibility means that container must have
    overloaded operator ObjectPtr& operator[](const Key& k) which returns ObjectPtr reference associated with the Key value.
    If there is no ObjectPtr instance associated with the Key value, than operator must add one to the container and return
    reference to it.

    Template parameters.

    Dataset - see DBReader.

    Object - see DBReader2ContainerBase.

    Container is the associative container of pairs <Key, ObjectPtr> from STL library (for example, std::map<Key, ObjectPtr>) or
    compatible container.

    Key is the type of the unique key used to identify instances of Object. If Key is a class then it must have default
    constructor.

    ObjectPtr is a class of smart pointer to Object. ObjectPtr must have constructor of the form
    ObjectPtr(Object* obj). The example of smart pointer is STL shared_ptr template class. Simple
    pointer Object* can be used as ObjectPtr too, but care must be taken to deallocate elements before
    destruction of the container.
*/
template <class Dataset, class Object, class Container, class Key, class ObjectPtr>
class DBReader2STLAssosiativePtr : public DBReader2Associative<Dataset, Object, Container, Key> {
public:
    /*!
        Short alias for DBReader2Associative<Dataset, Object, Container, Key> type.
    */
    typedef DBReader2Associative<Dataset, Object, Container, Key> AncestorType;
    /*!
        Constructs DBReader2STLAssosiative without assosiated container and DBReader2Object instances for Key and Object.
    */
    DBReader2STLAssosiativePtr() : DBReader2Associative<Dataset, Object, Container, Key>() {};
    /*!
        Constructs DBReader2STLAssosiative with assosiated container and DBReader2Object instances for Key and Object.
        @param[in] container Pointer to the Associative container that is used to store read data. The DBReader2STLAssosiative doesn't take
        ownership of container.
        @param[in] objectReader Pointer to the DBReader2Object descendant instance that is used read Object data. The DBReader2STLAssosiative
        doesn't take ownership of the objectReader.
        @param[in] keyReader Pointer to the DBReader2Object descendant instance that is used to read Key data. The DBReader2STLAssosiative
        doesn't take ownership of the keyReader.
    */
    DBReader2STLAssosiativePtr(Container* data, typename AncestorType::Reader2ObjectType* objectReader, typename AncestorType::Reader2KeyType* keyReader) :
        DBReader2Associative<Dataset, Object, Container, Key>(data, objectReader, keyReader) {};
protected:
    /*!
        The DBReader2STLAssosiative implements objectByKey method using Container's operator [].
        @param key Key value for required Object instance.
        @return Pointer to Object instance.
    */
    Object* objectByKey(const Key& key)
    {        
        if ( (*AncestorType::m_container)[key] == nullptr) {
            (*AncestorType::m_container)[key] = ObjectPtr(new Object);
        }
        return &(*(*AncestorType::m_container)[key]);
    }

};


}

#endif // DBREADER2STLASSOCIATIVEPTR_H
