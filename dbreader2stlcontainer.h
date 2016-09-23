#ifndef DBREADER2STLCONTAINER_H
#define DBREADER2STLCONTAINER_H

#include "dbreader2container.h"

namespace dbframework {

/*!
    The DBReader2STLContainer template class is the implementation of DBReader2Container for
    the group of containers. This group includes containers which have push_back(const Object& ) method
    that adds Object to the container. This group includes some containers from STL library
    (for example, std::vector) and Qt library (for example, QVector).

    Template parameters.

    Dataset - see DBReader.

    Object - see DBReader2Container.

    Container is the class implementing the container of Object. Container must have push_back(const Object& ) method
    that adds Object to the container.
*/
template <class Dataset, class Object, class Container>
class DBReader2STLContainer : public DBReader2Container<Dataset, Object, Container>
{
protected:
    /*!
        The DBReader2STLContainer implementation of addToContainer uses push_back to add new element to the container.
        @param[in] obj  Pointer to the Object instance which must be added to the container.
    */
    void addToContainer(Object* obj)
    {
        if (obj != nullptr) {
            DBReader2Container<Dataset, Object, Container>::m_container->push_back(*obj);
            delete obj;
        }
    }
public:
    /*!
        Constructs DBReader2STLContainer without assosiated container and DBReader2Object instance.
    */
    DBReader2STLContainer():DBReader2Container<Dataset, Object, Container>(){};
    /*!
        Constructs DBReader2STLContainer with assosiated container and DBReader2Object instance.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2Container doesn't take
        ownership of container.
        @param[in] reader Pointer to the DBReader2Object instance used to read Object data. DBReader2Container doesn't take ownership
        of reader.
    */
    DBReader2STLContainer(Container* container, DBReader2Object<Dataset, Object>* reader):
        DBReader2Container<Dataset, Object, Container>(container, reader){};
};

}

#endif // DBREADER2STLCONTAINER_H
