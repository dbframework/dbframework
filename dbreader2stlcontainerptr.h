/*
Copyright (c) 2015, 2016 Sidorov Dmitry

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  
*/

#ifndef DBREADER2STLCONTAINERPTR_H
#define DBREADER2STLCONTAINERPTR_H

#include "dbreader2container.h"

namespace dbframework {

/*!
    The DBReader2STLContainerPtr template class is the implementation of DBReader2Container for
    the group of containers. This group includes containers which have push_back(const T& ) method
    that adds class T instance to the container. This group includes some containers from STL library
    (for example, std::vector) and Qt library (for example, QVector). Unlike DBReader2STLContainer, the
    DBReader2STLContainerPtr class operates with containers of smart pointers to Object.

    Template parameters.

    Dataset - see DBReader.

    Object - see DBReader2Container.

    Container is the class implementing the container of ObjectPtr. Container must have
    push_back(const ObjectPtr& ) method that adds ObjectPtr to the container.

    ObjectPtr is a class of smart pointer to Object. ObjectPtr must have constructor of the form
    ObjectPtr(Object* obj). The example of smart pointer is STL shared_ptr template class. Simple
    pointer Object* can be used as ObjectPtr too, but care must be taken to deallocate elements before
    destruction of the container.
*/
template <class Dataset, class Object, class Container, class ObjectPtr>
class DBReader2STLContainerPtr : public DBReader2Container<Dataset, Object, Container>
{
protected:
    /*!
        The DBReader2STLContainer implementation of addToContainer uses push_back to add new element to the container.
        @param[in] obj  Pointer to the Object instance which must be added to the container.
    */
    void addToContainer(Object* obj)
    {
        if (obj != nullptr) {
            DBReader2Container<Dataset, Object, Container>::m_container->push_back(ObjectPtr(obj));
        }
    }
public:
    /*!
        Constructs DBReader2STLContainerPtr without assosiated container and DBReader2Object instance.
    */
    DBReader2STLContainerPtr():DBReader2Container<Dataset, Object, Container>(){};
    /*!
        Constructs DBReader2STLContainerPtr with assosiated container and DBReader2Object instance.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2ContainerPtr doesn't take
        ownership of container.
        @param[in] reader Pointer to the DBReader2Object instance used to read Object data. DBReader2ContainerPtr doesn't take ownership
        of reader.
    */
    DBReader2STLContainerPtr(Container* container, typename DBReader2Container<Dataset, Object, Container>::Reader2ObjectType* reader):
        DBReader2Container<Dataset, Object, Container>(container, reader){};
};

}

#endif // DBREADER2STLCONTAINERPTR_H
