#ifndef DBREADER2CONTAINERPTR_H
#define DBREADER2CONTAINERPTR_H

#include "dbreader.h"
#include "dbreader2containerbase.h"

namespace dbframework {

/*!
    The DBReader2Container class is a db reader that reads data from the Dataset record to the object and adds
    this object to the container.

    Template parameters.

    Dataset is the class that provides access to the data (possibly retrieved by SQL query or stored procedure).
    It is supposed that the data is organized as some container of records. It is supposed that Dataset provides
    pointer to the current record and some way to navigate between records.

    Object is the class used to store data read from the dataset. Object must have default constructor.

    Container is the class implementing STL-style container of Object. Container must have push_back method
    with one parameter of type Object which adds Object copy to the container.    
*/
template <class Dataset, class Object, class Container, class ObjectPtr>
class DBReader2ContainerPtr : public DBReader2ContainerBase<Dataset, Object, Container> {
public:
    /*!
        Constructs db reader without container and db reader for Object.
    */
    DBReader2ContainerPtr()
        : DBReader2ContainerBase<Dataset, Object, Container>() {};
    /*!
        Constructs db reader.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2Container doesn't take
        ownership of container.
        @param[in] reader Pointer to the db reader used to read Object data. DBReader2Container doesn't take ownership
        of reader.
    */
    DBReader2ContainerPtr(Container* container, Reader2ObjectType* reader)
        : DBReader2ContainerBase<Dataset, Object, Container>(container, reader) {};
    /*!
        Creates instance of Object, reads data from dataset to it using Reader2Object instance and then adds Object instance
        to the container.
        @param[in] ds Dataset to read from.
        @return Returns true if success.
    */
    bool read(Dataset &ds)
    {
        ObjectPtr obj(new Object);

        if ((m_objectReader == nullptr) || (m_object == nullptr))
            return false;

        bool result = false;
        m_objectReader->setObject(&(*obj));
        if (m_objectReader->read(ds)) {
            m_object->push_back(obj);
            result = true;
        }
        return result;
    };    
};

};

#endif // DBREADER2CONTAINERPTR_H
