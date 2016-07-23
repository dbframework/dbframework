#ifndef DBREADERSORTED2CONTAINER_H
#define DBREADERSORTED2CONTAINER_H

#include "dbreader.h"
#include "dbreader2assosiativebase.h"

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
    container doesn't have Object assosiated to the provided key. STL assosiative containers meet these requirements.

    Key is the type of the unique key used to identify instances of Object.
*/

template <class Dataset, class Object, class Container, class Key>
class DBReaderSorted2Container : public DBReader2AssosiativeBase<Dataset, Object, Container, Key> {
private:
    typedef DBReader2AssosiativeBase<Dataset, Object, Container, Key> AncestorType;
public:
    /*!
        Constructs db reader without container and db readers for Key and Object.
    */
    DBReaderSorted2Container() : DBReader2AssosiativeBase<Dataset, Object, Container, Key>() {};
    /*!
        Constructs db reader.
        @param[in] container Pointer to the assosiative container that is used to store read data. The DBReader2Assosiative doesn't take
        ownership of container.
        @param[in] objectReader Pointer to the db reader that is used read Object data. The DBReader2Assosiative doesn't take
        ownership of db reader.
        @param[in] keyReader Pointer to the db reader that is used to read Key data. The DBReader2Assosiative doesn't take
        ownership of db reader.
    */
    DBReaderSorted2Container(Container* data, DBReader2Object<Dataset, Object>* objectReader, DBReader2Object<Dataset, Key>* keyReader) :
        DBReader2AssosiativeBase<Dataset, Object, Container, Key>(data, objectReader, keyReader) {};
    /*!
        Creates instance of Key and reads data from dataset to it using m_keyReader. Gains accsses to the Object instance using
        operator[] with read key and reads data to the Object instance using m_objectReader.
        @param[in] ds Dataset to read from.
        @return Returns true if success.
    */
    bool read(Dataset& ds)
    {
        Key k;

        if ((AncestorType::m_keyReader == nullptr) || (AncestorType::m_objectReader == nullptr) || (AncestorType::m_object == nullptr))
            return false;

        bool result = false;
        AncestorType::m_keyReader->setObject(&k);
        if (AncestorType::m_keyReader->read(ds)) {
            if (AncestorType::m_object->empty()) {
                AncestorType::m_object->push_back(Object());
            }
            else {
                Object* p = &AncestorType::m_object->back();
                if ((*p)() != k) {
                    AncestorType::m_object->push_back(Object());
                }
            }
            AncestorType::m_objectReader->setObject(&AncestorType::m_object->back());
            result = AncestorType::m_objectReader->read(ds);
        }
        return result;
    };
};

}

#endif // DBREADERRSORTED2CONTAINER_H