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

#ifndef DBREADER2CONTAINERWITHKEY_H
#define DBREADER2CONTAINERWITHKEY_H

#include "dbreader2containerbase.h"

namespace dbframework {

/*!
    The DBReader2ContainerWithKey template class is used as a base class when implementing descendants that require
    preliminary reading of the unique identifier of the object before storing the object into the container. The
    DBReader2ContainerWithKey provides method to assosiate it with the instance of the special descendant of the
    DBReader2Object. Usually you shouldn't inherit from DBReader2ContainerWithKey. It is for dbframework internal use.

    Template parameters.

    Dataset - see DBReader.

    Object - see DBReader2ContainerBase.

    Container - see DBReader2ContainerBase.

    Key is the type of the unique identifier of the Object. DBReader2ContainerWithKey descendants may impose restrictions
    on Key.
*/
template <class Dataset, class Object, class Container, class Key>
class DBReader2ContainerWithKey : public DBReader2ContainerBase<Dataset, Object, Container> {
public:
    /*!
        Type of DBReader2Object used to read unique identifier.
    */
    typedef DBReader2Object<Dataset, Key> Reader2KeyType;
protected:
    /*!
        DBReader2Object descendant used to read unique identifier.
    */
    Reader2KeyType *m_keyReader;
public:
    /*!
        Constructs DBReader2ContainerWithKey without assosiated container, Reader2ObjectType and Reader2KeyType instances.
    */
    DBReader2ContainerWithKey() : DBReader2ContainerBase<Dataset, Object, Container>(), m_keyReader(nullptr) {};
    /*!
        Constructs DBReader2ContainerWithKey with assosiated container, Reader2ObjectType and Reader2KeyType instances.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2ContainerWithKey doesn't take
        ownership of container.
        @param[in] objectReader Pointer to the Reader2ObjectType instance that is used read Object data. The DBReader2ContainerWithKey
        doesn't take ownership of objectReader.
        @param[in] keyReader Pointer to the Reader2KeyType instance that is used to read Key data. The DBReader2ContainerWithKey
        doesn't take ownership of db reader.
    */
    DBReader2ContainerWithKey(Container* data, typename DBReader2ContainerBase<Dataset, Object, Container>::Reader2ObjectType* objectReader,
        Reader2KeyType* keyReader) :
        DBReader2ContainerBase<Dataset, Object, Container>(data, objectReader), m_keyReader(keyReader) {};
    /*!
        Get assosiated Reader2KeyType instance.
        @return Pointer to the assosiated Reader2KeyType instance or nullptr if Reader2KeyType instance wasn't assosiated with this
        DBReader2ContainerWithKey instance.
     */
    Reader2KeyType* keyReader() {return m_keyReader;};
    /*!
        Assosiated Reader2KeyType instance with this DBReader2ContainerWithKey instance.
        @param[in] reader Pointer to the Reader2KeyType instance. The DBReader2ContainerWithKey doesn't take ownership of reader.
     */
    void setKeyReader(Reader2KeyType* reader) {m_keyReader = reader;};
};

}


#endif // DBREADER2CONTAINERWITHKEY_H
