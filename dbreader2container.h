#ifndef DBREADER2CONTAINER_H
#define DBREADER2CONTAINER_H

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
template <class Dataset, class Object, class Container>
class DBReader2Container : public DBReader2ContainerBase<Dataset, Object, Container> {
public:
    /*!
        Constructs db reader without container and db reader for Object.
    */
    DBReader2Container()
        : DBReader2ContainerBase<Dataset, Object, Container>() {};
    /*!
        Constructs db reader.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2Container doesn't take
        ownership of container.
        @param[in] reader Pointer to the db reader used to read Object data. DBReader2Container doesn't take ownership
        of reader.
    */
    DBReader2Container(Container* container, DBReader2Object<Dataset, Object>* reader)
        : DBReader2ContainerBase<Dataset, Object, Container>(container, reader) {};
    /*!
        Creates instance of Object, reads data from dataset to it using m_objectReader and then adds Object instance
        to the container.
        @param[in] ds Dataset to read from.
        @return Returns true if success.
    */
    bool read(Dataset &ds)
    {
        Object obj;

        if ((DBReader2ContainerBase<Dataset, Object, Container>::m_objectReader == nullptr) || (DBReader2ContainerBase<Dataset, Object, Container>::m_object == nullptr))
            return false;

        bool result = false;
        DBReader2ContainerBase<Dataset, Object, Container>::m_objectReader->setObject(&obj);
        if (DBReader2ContainerBase<Dataset, Object, Container>::m_objectReader->read(ds)) {
            DBReader2ContainerBase<Dataset, Object, Container>::m_object->push_back(obj);
            result = true;
        }
        return result;
    };   
};

};

#endif // DBREADER2CONTAINER_H
