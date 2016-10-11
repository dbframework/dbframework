/*
Copyright (c) 2015, 2016 Sidorov Dmitry

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

#ifndef DBREADER2INDEXEDSTLCONTAINERPTR_H
#define DBREADER2INDEXEDSTLCONTAINERPTR_H

#include "dbreader2associative.h"

namespace dbframework {

/*!
    The DBReader2IndexedSTLContainerPtr template class combines the properties of DBReader2STLContainerPtr
    and DBReader2STLAssociativePtr. Like DBReader2STLContainerPtr, the DBReader2IndexedSTLContainerPtr is intended to store data
    in the sequence containers. But unlike DBReader2STLContainerPtr, the DBReader2IndexedSTLContainerPtr doesn't put new ObjectPtr instances
    to the end of the container. Like DBReader2STLAssociativePtr, the DBReader2IndexedContainer uses associative container to implement
    internal index which is used to efficently find Object instance by its unique identifier.

    Like DBReader2STLAssociativePtr, DBReader2IndexedContainer is useful when reading results of a SQL query, containing fields from
    joined tables, related as one-to-many. Let's call these tables Master and Detail. Suppose that their records data is represented
    by classes MasterClass and DetailClass. Suppose that MasterSuperClass is  the descendant of MasterClass, containing a vector
    of DetailClass. Then the result of the query can be stored in sequence container of MasterSuperClass smart pointers. And
    DBReader2IndexedSTLContainerPtr descendant can be used to read data to that container. The use of DBReader2IndexedSTLContainerPtr
    in this case ensures that the DetailClass instatance filled from the record data will be added to the vector of
    corresponding MasterSuperClass instance.

    Template parameters.

    Dataset - see DBReader.

    Object - see DBReader2ContainerBase.

    Container is the class implementing container of ObjectPtr. Container must have push_back(const ObjectPtr& ) method that
    adds ObjectPtr to the container.

    Key is the type of the unique key used to identify instances of Object. If Key is a class then it must have default
    constructor.

    ObjectPtr is a class of smart pointer to Object. ObjectPtr must have constructor of the form
    ObjectPtr(Object* obj) and assignment operator. The example of smart pointer is STL shared_ptr template class. Simple
    pointer Object* can be used as ObjectPtr too, but care must be taken to deallocate elements before
    destruction of the container.

    Index is the associative container of pairs <Key, ObjectPtr> from STL library (for example, std::map<Key, ObjectPtr>) or
    compatible container. Compatibility means that container must have
    overloaded operator ObjectPtr& operator[](const Key& k) which returns ObjectPtr reference associated with the Key value.
    If there is no ObjectPtr instance associated with the Key value, than operator must add one to the container and return
    reference to it.
*/
template <class Dataset, class Object, class Container, class Key, class ObjectPtr, class Index>
class DBReader2IndexedSTLContainerPtr : public DBReader2Associative<Dataset, Object, Container, Key> {
private:
    typedef DBReader2Associative<Dataset, Object, Container, Key> AncestorType;    
    Index m_index;
public:
    /*!
        Constructs DBReader2IndexedSTLContainerPtr without assosiated container and DBReader2Object instances for Key and Object.
    */
    DBReader2IndexedSTLContainerPtr() : DBReader2Associative<Dataset, Object, Container, Key>() {};
    /*!
        Constructs DBReader2IndexedSTLContainerPtr with assosiated container and DBReader2Object instances for Key and Object.
        @param[in] container Pointer to the Associative container that is used to store read data. The DBReader2IndexedContainer doesn't take
        ownership of container.
        @param[in] objectReader Pointer to the DBReader2Object descendant instance that is used read Object data. The DBReader2IndexedContainer
        doesn't take ownership of the objectReader.
        @param[in] keyReader Pointer to the DBReader2Object descendant instance that is used to read Key data. The DBReader2IndexedContainer
        doesn't take ownership of the keyReader.
    */
    DBReader2IndexedSTLContainerPtr(Container* data, typename AncestorType::Reader2ObjectType* objectReader, typename AncestorType::Reader2KeyType* keyReader) :
        DBReader2Associative<Dataset, Object, Container, Key>(data, objectReader, keyReader) {};
    /*!
        Clears internal index. Call this method after reading each SQL query results when reusing same DBReader2IndexedSTLContainerPtr instance for multiple queries.
    */
    void clearIndex()
    {
        m_index = Index();
    };
protected:    
    /*!
        The DBReader2IndexedSTLContainerPtr implements objectByKey method using Index's operator [] and Container's method push_back.
        @param key Key value for required Object instance.
        @return Pointer to Object instance.
    */
    Object* objectByKey(const Key& key)
    {
        ObjectPtr p = m_index[key];
        if (p == nullptr) {
            p = ObjectPtr(new Object);
            m_index[key] = p;
            m_container->push_back(p);
        }
        return &(*p);
    };


};


}

#endif // DBREADER2INDEXEDSTLCONTAINERPTR_H
