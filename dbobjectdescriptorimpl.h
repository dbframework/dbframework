#ifndef DBOBJECTDESCRIPTORIMPL_H
/*
Copyright (c) 2017 Sidorov Dmitry

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

#define DBOBJECTDESCRIPTORIMPL_H

#include "dbobjectdescriptor.h"

namespace dbframework {

/*!
    The DBObjectDescriptorImpl template class implements all abstract methods of DBObjectDescriptor
    except the parameterName. Inherit from DBObjectDescriptorImpl and implement parameterName to get
    fully functional DBObjectDescriptor descendant. The DBObjectDescriptorImpl uses two containers of
    elements of type S to implement DBObjectDescriptor abstract methods.

    Template parameters.

    S is a string type that is used to represent table or field name. Must have default and copy constructors.

    I is an integer type that is used as index when accessing Container elements.

    Container is a container of elements of type S that supports direct access to elements by index of type I (like std::vector).
    Container must have:
       - default and copy constructors;
       - size() method returning the number of elements in the container that can be implicitly casted to I;
       - operator[](I index) method returning element reference by zero-based index.
*/
template<typename S, typename I, typename Container>
class DBObjectDescriptorImpl : public DBObjectDescriptor<S,I> {
private:
    Container m_keyFields;
    Container m_commonFields;
    S m_tableName;
public:
    /*!
        Create DBObjectDescriptorImpl object.
        @param[in] tableName Table name.
        @param[in] keyFields Container with the names of fields that form primary key.
        @param[in] commonFields Container with the names of fields that are not included in the primary key.
    */
    DBObjectDescriptorImpl(const S& tableName, Container& keyFields, Container& commonFields):
        m_tableName(tableName), m_keyFields(keyFields), m_commonFields(commonFields)
    {

    };
    /*!
        Get table name.
        @return Table name.
    */
    S tableName() const
    {
        return m_tableName;
    };
    /*!
        Get the number of fields that form table's primary key.
        @return The number of fields in the primary key.
    */
    I keyFieldCount() const
    {
        return m_keyFields.size();
    };
    /*!
        Get the number of fields that aren't included in the table's primary key.
        @return The number of fields not included in the primary key.
    */
    I commonFieldCount() const
    {
        return m_commonFields.size();
    };
    /*!
        Get the field name for the field from primary key.
        @param[in] index Field's zero-based index (0 <= index < keyFieldCount()). 
        @return The name of the field from primary key with the provided index.
    */
    S keyField(I index) const
    {
        return m_keyFields[index];
    };
    /*!
        Get the field name for the field that doesn't belong to primary key.
        @param[in] index Field's zero-based index (0 <= index < commonFieldCount()).
        @return The name of the field that doesn't belong to primary key with the provided index.
    */
    S commonField(I index) const
    {
        return m_commonFields[index];
    };
};

}

#endif // DBOBJECTDESCRIPTORIMPL_H
