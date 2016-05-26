#ifndef DBREADER2ASSOCIATIVE_H
#define DBREADER2ASSOCIATIVE_H

#include "dbreader.h"
#include "dbread2object.h"

namespace dbframework {

/*!
    The DBReader2Assosiative class is a db reader that reads data from the Dataset record to the object and adds
    this object to the assosiative container.

    Template parameters.

    Dataset is the class that provides access to the data (possibly retrieved by SQL query or stored procedure).
    It is supposed that the data is organized as some container of records. It is supposed that Dataset provides
    pointer to the current record and some way to navigate between records. Every record must contain the key that
    can be used to unique identify object which is supposed to store record data.

    Object is the class used to store data read from the dataset. Object must have default constructor.

    Container is the class implementing STL-style assosiative container of <Key, Object> pairs. Container must have
    [] operator which gives access to the Object by Key. [] operator must add Object instance to the container if
    container doesn't have Object assosiated to the provided key (default behavior of STL assosiative containers).

    Reader2Object must be DBReader2Object<Dataset, Object> descendant. It must have a constructor with one parameter
    of type Object*.

    Key is the type of the unique key used to identify instances of Object.

    Reader2Key is the DBReader2Object<Dataset, Key> descendant used to read the key from the dataset record.
*/
template <class Dataset, class Object, class Container, class Key>
class DBReader2Assosiative : public DBReader<Dataset> {
protected:
    /*!
        Pointer to the container where read data is stored.
    */
    Container* m_data;
    typedef DBReader2Object<Dataset, Key> Reader2KeyType;
    Reader2KeyType *m_keyReader;
    typedef DBReader2Object<Dataset, Object> Reader2ObjectType;
    Reader2ObjectType *m_objectReader;
public:
    /*!
        Constructs db reader.
        @param[in] container Pointer to the assosiative container that is used to store read data. The DBReader2Assosiative doesn't take
        ownership of container.
    */
    DBReader2Assosiative(Container* data, Reader2ObjectType* objectReader, Reader2KeyType* keyReader) :
        DBReader<Dataset>(), m_data(data), m_objectReader(objectReader), m_keyReader(keyReader) {};
    /*!
        Creates instance of Key and reads data from dataset to it using Reader2Key instance. Gains accsses to the Object instance using
        operator[] with read key and reads data to the Object instance using Reader2Object instance.
        @param[in] ds Dataset to read from.
        @return Returns true.
    */
    bool read(Dataset& ds)
    {
        Key k;        
        m_keyReader->setObject(&k);
        if (m_keyReader->read(ds)) {
            m_objectReader->setObject(&(*m_data)[k]);
            m_objectReader->read(ds);
        }
        return true;
    };
};

}

#endif // DBREADER2ASSOCIATIVE_H
