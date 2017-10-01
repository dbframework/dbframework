#ifndef DBSQLGENERATOR_H
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

#define DBSQLGENERATOR_H

#include "dbobjectdescriptor.h"

namespace dbframework {

/*!
    The DBSQLGenerator template class provides the abstract interface for classes that generate
    a text of SQL query.

    Template parameters.

    S is a string type that is used to represent table or field name and query text.
    I is an integer type that is used as an index.
*/
template<typename S, typename I>
class DBSQLGenerator {
public:
    /*!
        Virtual destructor. Destroys the DBSQLGenerator instance.
    */
    virtual ~DBSQLGenerator() {};
    /*!
        This method must generate query for inserting object's data into a table.
        @param[in] d Pointer to DBObjectDescriptor instance that describes the table. DBSQLGenerator doesn't take
        ownership of d.
        @param[in] withKeyFields If true the method must include primary key fields into the query. Otherwise it mustn't
        include primary key fields into the query.
        @return SQL query text.
    */
    virtual S insert(DBObjectDescriptor<S, I>* d, bool withKeyFields) const = 0;
    /*!
        This method must generate query for updating object's data in a table.
        @param[in] d Pointer to DBObjectDescriptor instance that describes the table. DBSQLGenerator doesn't take
        ownership of d.    
        @return SQL query text.
    */
    virtual S update(DBObjectDescriptor<S, I>* d) const = 0;
    /*!
        This method must generate query for deleting object's data from a table.
        @param[in] d Pointer to DBObjectDescriptor instance that describes the table. DBSQLGenerator doesn't take
        ownership of d.
        @return SQL query text.
    */
    virtual S del(DBObjectDescriptor<S, I>* d) const = 0;
};

}
#endif // DBSQLGENERATOR_H
