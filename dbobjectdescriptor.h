#ifndef DBOBJECTDESCRIPTOR_H
/*
Copyright (c) 2015-2017 Sidorov Dmitry

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

#define DBOBJECTDESCRIPTOR_H

namespace dbframework {

/*!
    The DBObjectDescriptor template class provides the abstract interface for classes that describe 
    how some object is stored in the database. The DBObjectDescriptor descendants are used go generate 
    SQL queries.

    Template parameters.

    S is a string type that is used to represent table or field name.
    I is an integer type that is used as an index.
*/
template<typename S, typename I>
class DBObjectDescriptor {
public:
    /*!
        Virtual destructor. Destroys the DBObjectDescriptor instance.
    */
    virtual ~DBObjectDescriptor() {};
    /*!
        This method must return the name of the table where the object is stored.
        @return Table name.
    */
    virtual S tableName() const = 0;
    /*!
        This method must return the number of fields that form table's primary key.
        @return The number of fields in the primary key.
    */
    virtual I keyFieldCount() const = 0;
    /*!
        This method must return the number of fields that aren't included in the table's primary key.
        @return The number of fields not included in the primary key.
    */
    virtual I commonFieldCount() const = 0;
    /*!
        This method must return the field name for the field from primary key.
        @param[in] index Field's zero-based index (0 <= index < keyFieldCount()). 
        @return The name of the field from primary key with the provided index.
    */
    virtual S keyField(I index) const = 0;
    /*!
        This method must return the field name for the field that doesn't belong to primary key.
        @param[in] index Field's zero-based index (0 <= index < commonFieldCount()).
        @return The name of the field that doesn't belong to primary key with the provided index.
    */
    virtual S commonField(I index) const = 0;
    /*!
        This method must return the name for the parameter of SQL query, which value will be stored in the field
        with the provided name.
        @param[in] fieldName Name of the field that will store the value.
        @return The name of the parameter of SQL query which value will be stored in the field fieldName. 
    */
    virtual S parameterName(const S& fieldName) const = 0;
};

}

#endif // DBOBJECTDESCRIPTOR_H
