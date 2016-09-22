#ifndef DBREADER2CONTAINERBASE_H
#define DBREADER2CONTAINERBASE_H

#include "dbreader.h"
#include "dbread2object.h"

namespace dbframework {

/*!
    The DBReader2ContainerBase template class is a base class for DBReader descendants that read SQL query execution
    result and store it in the container of objects. Usually you shouldn't inherit from DBReader2ContainerBase.
    It is for dbframework internal use.

    DBReader2ContainerBase descendants must implement DBReader::read method to to read data from the current record of the
    SQL query execution result and store it in the fields of one of the objects in the container.

    Template parameters.

    Dataset - see DBReader.

    Object is the class which instances are stored in the container and are used to store data. DBReader2ContainerBase descendants
    may impose restrictions on Object.

    Container is the class implementing container of Object. DBReader2ContainerBase descendants may impose restrictions on Container.
*/
template <class Dataset, class Object, class Container>
class DBReader2ContainerBase : public DBReader<Dataset> {
protected:
    /*!
        Pointer to the container used to store data.
    */
    Container* m_container;
    /*!
        Type of the DBReader descendant used to read data and store it in the Object instance.
     */
    typedef DBReader2Object<Dataset, Object> Reader2ObjectType;
    /*!
        Pointer to DBReader descendant used read data and store it in the Object instance.
     */
    Reader2ObjectType* m_objectReader;
public:
    /*!
        Constructs DBReader2ContainerBase without assosiated container and Reader2ObjectType instance.
    */
    DBReader2ContainerBase()
        : DBReader<Dataset>(), m_container(nullptr), m_objectReader(nullptr) {};
    /*!
        Constructs DBReader2ContainerBase with assosiated container and Reader2ObjectType instance.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2Container doesn't take
        ownership of the container.
        @param[in] reader Pointer to the Reader2ObjectType instance used to read Object data. DBReader2Container doesn't take ownership
        of the reader.
    */
    DBReader2ContainerBase(Container* container, Reader2ObjectType* reader)
        : DBReader<Dataset>(), m_container(container), m_objectReader(reader) {};
    /*!
        Get pointer to the container used to store data.
        @return Pointer to the container used to store data or nullptr if container wasn't assosiated.
    */
    Container* container() {return m_container;};
    /*!
        Assosiate container used to store data with DBReader2ContainerBase instance.
        @param[in] container Pointer to the container used to store data. The DBReader2Container doesn't take
        ownership of the container.
    */
    void setContainer(Container* container){m_container = container;};
    /*!
        Get Reader2ObjectType instance used to read Object data.
        @return Pointer to Reader2ObjectType instance used to read Object data or nullptr if Reader2ObjectType instance wasn't
        assosiated with DBReader2ContainerBase instance.
    */
    Reader2ObjectType* reader() {return m_objectReader;};
    /*!
        Assosiate Reader2ObjectType instance used to read Object data with DBReader2ContainerBase instance.
        @param[in] reader Pointer Reader2ObjectType instance. DBReader2Container doesn't take ownership of reader.
    */
    void setReader(Reader2ObjectType* reader) {m_objectReader = reader;};
};

};


#endif // DBREADER2CONTAINERBASE_H
