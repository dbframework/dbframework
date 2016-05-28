#ifndef DBREADER2CONTAINERBASE_H
#define DBREADER2CONTAINERBASE_H

#include "dbreader.h"
#include "dbread2object.h"

namespace dbframework {

/*!
    The DBReader2ContainerBase class is a base class for db readers that read data from the Dataset record to the object and add
    this object to the container.

    Template parameters.

    Dataset is the class that provides access to the data (possibly retrieved by SQL query or stored procedure).
    It is supposed that the data is organized as some container of records. It is supposed that Dataset provides
    pointer to the current record and some way to navigate between records.

    Object is the class used to store data read from the dataset. Object must have default constructor.

    Container is the class implementing STL-style container of Object. Container must have push_back method
    with one parameter of type Object which adds Object copy to the container.
*/
template <class Dataset, class Object, class Container>
class DBReader2ContainerBase : public DBReader2Object<Dataset, Container> {
protected:
    /*!
        Type of the db reader used to read Object data.
     */
    typedef DBReader2Object<Dataset, Object> Reader2ObjectType;
    /*!
        Db reader used to read Object data.
     */
    Reader2ObjectType* m_objectReader;
public:
    /*!
        Constructs db reader without container and db reader for Object.
    */
    DBReader2ContainerBase()
        : DBReader2Object<Dataset, Container>(), m_objectReader(nullptr) {};
    /*!
        Constructs db reader.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2Container doesn't take
        ownership of container.
        @param[in] reader Pointer to the db reader used to read Object data. DBReader2Container doesn't take ownership
        of reader.
    */
    DBReader2ContainerBase(Container* container, Reader2ObjectType* reader)
        : DBReader2Object<Dataset, Container>(container), m_objectReader(reader) {};
    /*!
        Get db reader used to read Object data.
        @return Pointer to db reader used to read Object data.
    */
    Reader2ObjectType* reader() {return m_objectReader;};
    /*!
        Set db reader used to read Object data.
        @param[in] reader Pointer to db reader. DBReader2Container doesn't take ownership of reader.
    */
    void setReader(Reader2ObjectType* reader) {m_objectReader = reader;};
};

};


#endif // DBREADER2CONTAINERBASE_H
