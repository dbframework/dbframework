#ifndef DBREADER2ASSOCIATIVE_H
#define DBREADER2ASSOCIATIVE_H

#include "dbreader2containerwithkey.h"

namespace dbframework {

/*!
    The DBReader2Associative class is the base class for implementing DBReader descendants that read SQL query execution
    result and store it in the Associative container (like STL library map and unordered_map). The reading algorithm is:
    - read object unique identifier;
    - get pointer to object stored in the container by its identifier;
    - read data to the object fields.

    To adapt DBReader2Associative to some container its descendant must implement objectByKey method.

    DBReader2Associative is useful when reading results of a SQL query, containing fields from  joined tables, related as
    one-to-many. Let's call these tables Master and Detail. Suppose that their records data is represented by classes
    MasterClass and DetailClass. Suppose that MasterSuperClass is  the descendant of MasterClass, containing a vector
    of DetailClass. Then the result of the query can be stored in Associative container of MasterSuperClass. And
    DBReader2Associative descendant can be used to read data to that Associative container. The use of Associative container
    in this case ensures that the DetailClass instatnce filled from the record data will be added to the vector of
    corresponding MasterSuperClass instance.

    Template parameters.

    Dataset - see DBReader.

    Object - see DBReader2ContainerBase.

    Container is the class implementing Associative container, which makes it possible to efficently access Object instance
    by Key value.

    Key is the type of the unique key used to identify instances of Object. If Key is a class then it must have default
    constructor.
*/
template <class Dataset, class Object, class Container, class Key>
class DBReader2Associative : public DBReader2ContainerWithKey<Dataset, Object, Container, Key> {
private:
    typedef DBReader2ContainerWithKey<Dataset, Object, Container, Key> AncestorType;
protected:
    /*!
        The objectByKey objectByKey method must be implemented in the descendants. It must find the Object instance by the Key value
        and return pointer to it. If container doesn't has Object with the Key value, objectByKey must add Object instance to the container
        and return pointer to it.
        @param key Key value for required Object instance.
        @return Pointer to Object instance.
    */
    virtual Object* objectByKey(const Key& key) = 0;
public:
    /*!
        Constructs DBReader2Associative without assosiated container and DBReader2Object instances for Key and Object.
    */
    DBReader2Associative() : DBReader2ContainerWithKey<Dataset, Object, Container, Key>() {};
    /*!
        Constructs DBReader2Associative with assosiated container and DBReader2Object instances for Key and Object.
        @param[in] container Pointer to the Associative container that is used to store read data. The DBReader2Associative doesn't take
        ownership of container.
        @param[in] objectReader Pointer to the DBReader2Object descendant instance that is used read Object data. The DBReader2Associative
        doesn't take ownership of the objectReader.
        @param[in] keyReader Pointer to the DBReader2Object descendant instance that is used to read Key data. The DBReader2Associative
        doesn't take ownership of the keyReader.
    */
    DBReader2Associative(Container* data, typename AncestorType::Reader2ObjectType* objectReader, typename AncestorType::Reader2KeyType* keyReader) :
        DBReader2ContainerWithKey<Dataset, Object, Container, Key>(data, objectReader, keyReader) {};
    /*!
        Creates instance of Key and reads data from Dataset instance current record to it using m_keyReader. Gains accsses to the Object instance using
        objectByKey and reads data to the Object instance using m_objectReader.
        @param[in] ds Dataset to read from.
        @return Returns true if success.
    */
    bool read(Dataset& ds)
    {
        if ((AncestorType::m_keyReader == nullptr) || (AncestorType::m_objectReader == nullptr) || (AncestorType::m_container == nullptr))
            return false;

        Key k;
        bool result = false;

        AncestorType::m_keyReader->setObject(&k);
        if (AncestorType::m_keyReader->read(ds)) {
            AncestorType::m_objectReader->setObject(objectByKey(k));
            result = AncestorType::m_objectReader->read(ds);
        }
        return result;
    };    
};

}

#endif // DBREADER2ASSOCIATIVE_H
