#ifndef DBSQLGENERATORIMPL_H
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

#define DBSQLGENERATORIMPL_H

#include "dbsqlgenerator.h"

namespace dbframework {
/*!
    The DBSQLGeneratorImpl template class is a DBSQLGenerator implementation.

    Template parameters.

    S is a "smart" string type that is used to represent table or field name and query text. Type S must have:
    - default constructor;
    - constructor with one parameter of type char* that constructs S and initializes from char*;
    - += operator that appends S on the right to S on the left.
    I is an integer type that is used as an index.

    Consider following tables that will be used for illustrative examples.
    Table1 has four fields: id1, id2, value1, value2. Fields id1 and id2 form primary key.
    Table2 has two fields: id1, id2. Fields id1 and id2 form primary key.
    Table3 has two fields: value1, value2 and no primary key.

    In the examples of method implementations it is supposed that DBObjectDescriptor<S, I>* d has
    the implementation of parameterName(const S& fieldName) method that adds prefix ":" to fieldName.
*/
template<typename S, typename I>
class DBSQLGeneratorImpl : public DBSQLGenerator<S,I> {
private:
    S keyFieldStr(I i, DBObjectDescriptor<S, I>* d) const
    {
        S result;
        result += d->keyField(i);
        result += S(" = ");
        result += d->parameterName(d->keyField(i));
        return result;
    };
    S commonFieldStr(I i, DBObjectDescriptor<S, I>* d) const
    {
        S result;
        result += d->commonField(i);
        result += S(" = ");
        result += d->parameterName(d->commonField(i));
        return result;
    };
public:    
    /*!
        This method generates query for inserting object's data into a table. For example tables it generates following 
        queries.
        Table1 (withKeyFields == true): insert into Table1(id1, id2, value1, value2) values (:id1, :id2, :value1, :value2) 
        Table1 (withKeyFields == false): insert into Table1(value1, value2) values (:value1, :value2)
        Table2 (withKeyFields == true): insert into Table2(id1, id2) values (:id1, :id2)
        Table2 (withKeyFields == false): returns S()
        Table3: insert into Table3(value1, value2) values (:value1, :value2)
        @param[in] d Pointer to DBObjectDescriptor instance that describes the table. DBSQLGenerator doesn't take
        ownership of d.
        @param[in] withKeyFields If true the method must include primary key fields into the query. Otherwise it mustn't
        include primary key fields into the query.
        @return SQL query text, see examples in method description.        
    */
    S insert(DBObjectDescriptor<S, I>* d, bool withKeyFields) const
    {
        if (!withKeyFields && (d->commonFieldCount() == 0))
            return S();
        if (withKeyFields && (d->commonFieldCount() + d->keyFieldCount() == 0))
            return S();

        S result("insert into ");
        result += d->tableName();
        result += S("(");
        I i;

        if (withKeyFields) {
            for (i = 0; i < d->keyFieldCount() - 1; ++i) {
                result += d->keyField(i);
                result += S(", ");
            }
            if (i < d->keyFieldCount()) {
                result += d->keyField(i);
                if (d->commonFieldCount() > 0) {
                    result += S(", ");
                }
            }
        }
        for (i = 0; i < d->commonFieldCount() - 1; ++i) {
            result += d->commonField(i);
            result += S(", ");
        }
        if (i < d->commonFieldCount()) {
            result += d->commonField(i);            
        }
        result += S(") values (");

        if (withKeyFields) {
            for (i = 0; i < d->keyFieldCount() - 1; ++i) {
                result += d->parameterName(d->keyField(i));
                result += S(", ");
            }
            if (i < d->keyFieldCount()) {
                result += d->parameterName(d->keyField(i));
                if (d->commonFieldCount() > 0) {
                    result += S(", ");
                }
            }
        }
        for (i = 0; i < d->commonFieldCount() - 1; ++i) {
            result += d->parameterName(d->commonField(i));
            result += S(", ");
        }
        if (i < d->commonFieldCount()) {
            result += d->parameterName(d->commonField(i));
        }
        result += S(")");

        return result;
    };
    /*!
        This method generates query for updating object's data in a table. For example tables it generates following 
        queries.
        Table1: update Table1 set value1 = :value1, value2 = :value2 where id1 = :id1 and id2 =: id2        
        Table2: returns S()        
        Table3: update Table3 set value1 = :value1, value2 = :value2 
        @param[in] d Pointer to DBObjectDescriptor instance that describes the table. DBSQLGenerator doesn't take
        ownership of d.
        @param[in] withKeyFields If true the method must include primary key fields into the query. Otherwise it mustn't
        include primary key fields into the query.
        @return SQL query text, see examples in method description. 
    */
    S update(DBObjectDescriptor<S, I>* d) const
    {
        if (d->commonFieldCount() == 0)
            return S();

        S result("update "); 
        result += d->tableName();
        result += S(" set ");
        I i;

        for (i = 0; i < d->commonFieldCount() - 1; ++i) {
            result += commonFieldStr(i,d);
            result += S(", ");
        }
        if (i < d->commonFieldCount()) {
            result += commonFieldStr(i,d);            
        }

        if (d->keyFieldCount() > 0) {
            result += S(" where ");

            for (i = 0; i < d->keyFieldCount() - 1; ++i) {
                result += keyFieldStr(i,d);
                result += S(" and ");
            }            
            result += keyFieldStr(i,d);
        }
        return result;
    };
    /*!
        This method generates query for deleting object's from a table. For example tables it generates following
        queries.
        Table1: delete from Table1 where id1 = :id1 and id2 = :id2
        Table2: delete from Table2 where id1 = :id1 and id2 = :id2
        Table3: delete from Table3
        @param[in] d Pointer to DBObjectDescriptor instance that describes the table. DBSQLGenerator doesn't take
        ownership of d.
        @param[in] withKeyFields If true the method must include primary key fields into the query. Otherwise it mustn't
        include primary key fields into the query.
        @return SQL query text, see examples in method description.
    */
    S del(DBObjectDescriptor<S, I>* d) const
    {
        S result("delete from ");
        result += d->tableName();
        I i;

        if (d->keyFieldCount() > 0) {
            result += S(" where ");
            for (i = 0; i < d->keyFieldCount() - 1; ++i) {
                result += keyFieldStr(i,d);                
                result += S(" and ");
            }
            result += keyFieldStr(i,d);            
        }
        return result;
    };
};

}

#endif //DBSQLGENERATORIMPL_H
