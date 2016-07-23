#ifndef DBREADER2ASSOSIATIVEBASE_H
#define DBREADER2ASSOSIATIVEBASE_H

#include "dbreader2containerbase.h"

namespace dbframework {

/*!
    The DBReader2AssosiativeBase class is a base class for db readers that read data from the Dataset record to the object
    and add this object to the assosiative container.

    Template parameters.

    Dataset is the class that provides access to the data (possibly retrieved by SQL query or stored procedure).
    It is supposed that the data is organized as some container of records. It is supposed that Dataset provides
    pointer to the current record and some way to navigate between records. Every record must contain the key that
    can be used to unique identify object which is supposed to store record data.

    Object is the class used to store data read from the dataset. Object must have default constructor.

    Container is the class implementing STL-style assosiative container of <Key, Object> pairs. Container must have
    [] operator which gives access to the Object by Key. [] operator must add Object instance to the container if
    container doesn't have Object assosiated to the provided key. STL assosiative containers meet these requirements.

    Key is the type of the unique key used to identify instances of Object.
*/
template <class Dataset, class Object, class Container, class Key>
class DBReader2AssosiativeBase : public DBReader2ContainerBase<Dataset, Object, Container> {
public:
    /*!
        Type of the db reader used to read Key.
    */
    typedef DBReader2Object<Dataset, Key> Reader2KeyType;
protected:
    /*!
       DB reader used to read Key.
    */
    Reader2KeyType *m_keyReader;
public:
    /*!
        Constructs db reader without container and db readers for Key and Object.
    */
    DBReader2AssosiativeBase() : DBReader2ContainerBase<Dataset, Object, Container>(), m_keyReader(nullptr) {};
    /*!
        Constructs db reader.
        @param[in] container Pointer to the assosiative container that is used to store read data. The DBReader2AssosiativeBase doesn't take
        ownership of container.
        @param[in] objectReader Pointer to the db reader that is used read Object data. The DBReader2AssosiativeBase doesn't take
        ownership of db reader.
        @param[in] keyReader Pointer to the db reader that is used to read Key data. The DBReader2AssosiativeBase doesn't take
        ownership of db reader.
    */
    DBReader2AssosiativeBase(Container* data, DBReader2Object<Dataset, Object>* objectReader, Reader2KeyType* keyReader) :
        DBReader2ContainerBase<Dataset, Object, Container>(data, objectReader), m_keyReader(keyReader) {};
    /*!
        Get db reader that is used to read Key data.
        @return Pointer to the db reader that is used to read Key data.
     */
    Reader2KeyType* keyReader() {return m_keyReader;};
    /*!
        Set db reader that is used to read Key data.
        @param[in] reader Pointer to the db reader that is used to read Key data. The DBReader2AssosiativeBase doesn't take
        ownership of db reader.
     */
    void setKeyReader(Reader2KeyType* reader) {m_keyReader = reader;};
};

}


#endif // DBREADER2ASSOSIATIVEBASE_H
