/*
Copyright (c) 2015 Sidorov Dmitry

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

#ifndef DBDMLQUERY_H
#define DBDMLQUERY_H

#include "dbobjectwrapper.h"

namespace dbframework {

/*!
    The DBDetailWrapper is a class template implementing generation of some of the DML 
    queries based on table and field info from object wrapper. Class methods 
    generats queries with parameters that have names of the following form:
    <prefix><table name><separator><field name><suffix>.
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    String - class implementing string. It should have default constructor, constructor
    String(char* str), assignment operator and operator+ defined as string concatenation.
    Key - class implementing object unique identifier. See DBObjectWrapper description for
    details.
    
    Usage scenario.
    Create instance of DBDMLQuery with proper template parameters and call it's methods 
    to get DML queries.
*/
template<class Dataset, class String, class Key>
class DBDMLQuery
{
private:
    String m_prefix;
    String m_separator;
    String m_suffix;
    bool m_addTableName;    
public:
    /*!
        Creates class instance with following parameters:
        prefix = String(":");
        separator = String("_");
        suffix = String();
        addTableName = true;
     */
    DBDMLQuery()
    {
        m_prefix = String(":");
        m_separator = String("_");
        m_suffix = String();
        m_addTableName = true;
    };
    /*!
        Gets parameter name prefix.
        @return Parameter name prefix.
     */
    String prefix() const
    {
        return m_prefix;
    }
    /*!
        Gets parameter name separator.
        @return Parameter name separator.
     */
    String separator() const
    {
        return m_separator;
    }
    /*!
        Gets parameter name suffix.
        @return Parameter name suffix.
     */
    String suffix() const
    {
        return m_suffix;
    }
    /*!
        Checks if table name will be added to the parameters names.
        @return True if table name will be added.
     */
    bool addTableName() const
    {
        return m_addTableName;
    }
    /*!
        Sets parameter name prefix.
        @param[in] value Parameter name prefix.
     */
    void setPrefix(const String& value)
    {
        m_prefix = value;
    }
    /*!
        Sets parameter name separator().
        @param[in] value Parameter name separator.
     */
    void setSeparator(const String& value)
    {
        m_separator = value;
    }
    /*!
        Sets parameter name suffix().
        @param[in] value Parameter name suffix.
     */
    void setSuffix(const String& value)
    {
        m_suffix = value;
    }
    /*!
        Sets if DBDMLQuery should add table name to parameters names.
        @param[in] value True to add table name, false to not.
     */
    void setAddTableName(bool value)
    {
        m_addTableName = value;
    }

    /*!
        Generates insert query based on object wrapper table data. Generates insert query of the form:
        
        insert into <table name>(<field[0] name>, ... <field[N-1] name>) values (
        <prefix><table name><separator><field[0] name><suffix>, ..., <prefix><table name><separator><field[N-1] name><suffix>)
        
        where N is the field count (or non-key field count).        
        @param[in] binder Object wrapper to use.
        @param[in] withKeyFields If true, key fields (forming primary key) will be included into the query. This is usefull 
        when primary key values must be assigned by the query. If false key fields will not be included. This is usefull 
        when primary key is defined automatically by the DBMS.
        @return Query text or empty string in case of error.
     */
    String insertQuery(const DBObjectWrapperAbstract<Dataset, String, Key>& binder, bool withKeyFields)
    {
        String result;
        if (binder.fieldCount() + binder.keyFieldCount() > 0) {
            result = String("insert into ") + binder.table() + String("(");        
            for (int i = 0; (i < binder.keyFieldCount()) && withKeyFields; ++i) {
                result = result + binder.keyField(i);
                if ((i < binder.keyFieldCount() - 1) || (binder.fieldCount() > 0) )
                    result = result + String(",");
            }
            for (int i = 0; i < binder.fieldCount(); ++i) {
                result = result + binder.field(i);
                if (i < binder.fieldCount() - 1)
                    result = result + String(",");
            }
            result = result + String(") values (");
            for (int i = 0; (i < binder.keyFieldCount()) && withKeyFields; ++i) {
                result = result + m_prefix + (m_addTableName?binder.table():String()) + m_separator + binder.keyField(i) + m_suffix;
                if ((i < binder.keyFieldCount() - 1) || (binder.fieldCount() > 0) )
                    result = result + String(",");
            }
            for (int i = 0; i < binder.fieldCount(); ++i) {
                result = result + m_prefix + (m_addTableName?binder.table():String()) + m_separator + binder.field(i) + m_suffix;
                if (i < binder.fieldCount() - 1)
                    result = result + String(",");
            }
            result = result + String(")");
        }
        return result;
    }
    
    /*!      
        Generates update query based on object wrapper table data. Generates update query of the form:
        
        update <table name> set <field[0] name> = <prefix><table name><separator><field[0] name><suffix>, ...,
        <field[N-1] name> = <prefix><table name><separator><field[N-1] name><suffix> where 
        <field[N] name> = <prefix><table name><separator><field[N] name><suffix> and ...
        <field[N+K-1] name> = <prefix><table name><separator><field[N+K-1] name><suffix>
        
        where N is the number of non-key fields, K is the number of key fields (which form primary key).        
        @param[in] binder Object wrapper to use.        
        @return Query text or empty string in case of error.
     */
    String updateQuery(const DBObjectWrapperAbstract<Dataset, String, Key>& binder)
    {
        String result;
        if ((binder.keyFieldCount() > 0) && (binder.fieldCount() > 0)) {
            result = String("update ") + binder.table() + String(" set ");
            for (int i = 0; i < binder.fieldCount(); ++i) {
                result = result + binder.field(i) + String(" = ") + m_prefix + 
                        (m_addTableName?binder.table():String()) + m_separator + binder.field(i) + m_suffix;
                if (i < binder.fieldCount() - 1)
                    result = result + String(",");
            }
            result = result + String(" where ");
            for (int i = 0; (i < binder.keyFieldCount()); ++i) {
                result = result + binder.keyField(i) + String(" = ") + m_prefix + 
                        (m_addTableName?binder.table():String()) + m_separator + binder.keyField(i) + m_suffix;
                if (i < binder.keyFieldCount() - 1)
                    result = result + String(" and ");
            }
        }
        return result;
    }
    /*!
        Generates delete query based on object wrapper table data. Generates dalete query of the form:
        
        delete from <table name> where <field[0] name> = <prefix><table name><separator><field[0] name><suffix> and ...
        <field[N-1] name> = <prefix><table name><separator><field[N-1] name><suffix> 
        
        where N is the number of key fields (which form primary key).        
        @param[in] binder Object wrapper to use.        
        @return Query text or empty string in case of error.
     */
    String deleteQuery(const DBObjectWrapperAbstract<Dataset, String, Key>& binder)
    {
        String result;
        if (binder.keyFieldCount() > 0) {
            result = String("delete from ") + binder.table() + String(" where ");
            for (int i = 0; (i < binder.keyFieldCount()); ++i) {
                result = result + binder.keyField(i) + String(" = ") + m_prefix + binder.table() + m_separator + 
                        binder.keyField(i) + m_suffix;
                if (i < binder.keyFieldCount() - 1)
                    result = result + String(" and ");
            }
        }
        return result;
    }    
};

}

#endif // DBDMLQUERY_H
