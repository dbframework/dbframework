#ifndef DBREADER2CONTAINER_H
#define DBREADER2CONTAINER_H

#include "dbreader.h"
#include "dbreader2containerbase.h"

namespace dbframework {

/*!
    The DBReader2Container class is the descendant of DBReader that implements the specific algorithm
    of reading SQL query execution result and storing it in the container of objects. The algorithm is:
    - allocate Object instance;
    - read data to the fields of the Object instance;
    - add object instance to the container.
    Container becomes filled with objects containing read data after Calling DBReader2Container::read
    for all records of SQL query execution result .

    The DBReader2Container class is useful when:
    - making SQL query from one table (or tables realted as one-to-one);
    - making SQL query from tables with one-to-many relation where Object corresponds to the second table
    ("many").

    The DBReader2Container class is the base class for descendants with the same reading algorithm but supporting
    different containers. To create a DBReader2Container descendant for specific container implement its
    addToContainer method, used to add new element to the container.

    Template parameters.

    Dataset - see DBReader.

    Object is the class which instances are stored in the container and are used to store data. Object must
    have default constructor.

    Container is the class implementing the container of Object.
*/
template <class Dataset, class Object, class Container>
class DBReader2Container : public DBReader2ContainerBase<Dataset, Object, Container> {
protected:
    /*!
        This method must be implemented in the descendants. It must add Object instance, pointed by obj,
        to the conteiner. If method implementation copies Object instance to the container and doesn't
        require it any more, it must free obj with delete.
        @param[in] obj Pointer to the Object instance which must be added to the container.
    */
    virtual void addToContainer(Object* obj) = 0;
public:
    /*!
        Constructs DBReader2Container without assosiated container and DBReader2Object instance.
    */
    DBReader2Container()
        : DBReader2ContainerBase<Dataset, Object, Container>() {};
    /*!
        Constructs DBReader2Container with assosiated container and DBReader2Object instance.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2Container doesn't take
        ownership of container.
        @param[in] reader Pointer to the DBReader2Object instance used to read Object data. DBReader2Container doesn't take ownership
        of reader.
    */
    DBReader2Container(Container* container, typename DBReader2ContainerBase<Dataset, Object, Container>::Reader2ObjectType* reader)
        : DBReader2ContainerBase<Dataset, Object, Container>(container, reader) {};
    /*!
        The DBReader2Container implementation of read method does following:
        - allocates instance of Object;
        - reads data to Object instance fields using assosiated DBReader2Object instance;
        - adds Object instance to the container by calling addToContainer.
        @param[in] ds Dataset to read from.
        @return Returns true if success.
    */
    bool read(Dataset &ds)
    {
        if ((DBReader2ContainerBase<Dataset, Object, Container>::m_objectReader == nullptr) || (DBReader2ContainerBase<Dataset, Object, Container>::m_container == nullptr))
            return false;

        Object* obj = new Object;

        bool result = false;
        DBReader2ContainerBase<Dataset, Object, Container>::m_objectReader->setObject(obj);
        if (DBReader2ContainerBase<Dataset, Object, Container>::m_objectReader->read(ds)) {            
            addToContainer(obj);
            result = true;
        }
        return result;
    };   
};

};

#endif // DBREADER2CONTAINER_H
