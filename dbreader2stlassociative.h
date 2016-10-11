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

#ifndef DBREADER2STLASSOCIATIVE_H
#define DBREADER2STLASSOCIATIVE_H

#include "dbreader2associative.h"

namespace dbframework {

/*!
    The DBReader2STLAssociative template class is the DBReader2Associative descendant that supports STL associative
    containers map and unordered_map and other compatible containers. Compatibility means that container must have
    overloaded operator Object& operator[](const Key& k) which returns Object reference associated with the Key value.
    If there is no Object instance associated with the Key value, than operator must add one to the container and return
    reference to it.

    Template parameters.

    Dataset - see DBReader.

    Object - see DBReader2ContainerBase.

    Container is the associative container of pairs <Key, Object> from STL library (for example, std::map<Key, Object>) or
    compatible container.

    Key is the type of the unique key used to identify instances of Object. If Key is a class then it must have default
    constructor.
*/
template <class Dataset, class Object, class Container, class Key>
class DBReader2STLAssociative : public DBReader2Associative<Dataset, Object, Container, Key> {
private:
    typedef DBReader2Associative<Dataset, Object, Container, Key> AncestorType;
public:
    /*!
        Constructs DBReader2STLAssociative without assosiated container and DBReader2Object instances for Key and Object.
    */
    DBReader2STLAssociative() : DBReader2Associative<Dataset, Object, Container, Key>() {};
    /*!
        Constructs DBReader2STLAssociative with assosiated container and DBReader2Object instances for Key and Object.
        @param[in] container Pointer to the Associative container that is used to store read data. The DBReader2STLAssociative doesn't take
        ownership of container.
        @param[in] objectReader Pointer to the DBReader2Object descendant instance that is used read Object data. The DBReader2STLAssociative
        doesn't take ownership of the objectReader.
        @param[in] keyReader Pointer to the DBReader2Object descendant instance that is used to read Key data. The DBReader2STLAssociative
        doesn't take ownership of the keyReader.
    */
    DBReader2STLAssociative(Container* data, typename AncestorType::Reader2ObjectType* objectReader, typename AncestorType::Reader2KeyType* keyReader) :
        DBReader2Associative<Dataset, Object, Container, Key>(data, objectReader, keyReader) {};
protected:
    /*!
        The DBReader2STLAssociative implements objectByKey method using Container's operator [].
        @param key Key value for required Object instance.
        @return Pointer to Object instance.
    */
    Object* objectByKey(const Key& key)
    {
        return &((*AncestorType::m_container)[key]);
    };

};


}

#endif // DBREADER2STLASSOCIATIVE_H
