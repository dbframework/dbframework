#ifndef DBDMLQUERY_H
#define DBDMLQUERY_H

#include "dbobjectwrapper.h"

/*!
    Class contains methods that generate standard DML queries based on info from DBObjectWrapper.
    Params will have names :<table name>_<field name>.
*/
template<class Dataset, class String, class Key>
class DBDMLQuery
{
public:
    DBDMLQuery(){};
    /*!
        Generate insert query.
        @param[in] binder DBObjectWrapper to use.
        @param[in] withKeyFields If true, key fields (primary key) will be included into the query.
        @return Query text or empty string in case of error.
     */
    static String insertQuery(const DBObjectWrapper<Dataset, String, Key>& binder, bool withKeyFields)
    {
        String result;
        if (binder.fieldCount() + binder.keyFieldCount() > 0) {
            result = "insert into " + binder.table() + "(";        
            for (int i = 0; (i < binder.keyFieldCount()) && withKeyFields; ++i) {
                result += binder.keyField(i);
                if ((i < binder.keyFieldCount() - 1) || (binder.fieldCount() > 0) )
                    result += ",";
            }
            for (int i = 0; i < binder.fieldCount(); ++i) {
                result += binder.field(i);
                if (i < binder.fieldCount() - 1)
                    result += ",";
            }
            result += ") values (";
            for (int i = 0; (i < binder.keyFieldCount()) && withKeyFields; ++i) {
                result += ":" + binder.table() + "_" + binder.keyField(i);
                if ((i < binder.keyFieldCount() - 1) || (binder.fieldCount() > 0) )
                    result += ",";
            }
            for (int i = 0; i < binder.fieldCount(); ++i) {
                result += ":" + binder.table() + "_" + binder.field(i);
                if (i < binder.fieldCount() - 1)
                    result += ",";
            }
            result += ")";
        }
        return result;
    }
    
    /*!
        Generate update query.
        @param[in] binder DBObjectWrapper to use.        
        @return Query text or empty string in case of error.
     */
    static String updateQuery(const DBObjectWrapper<Dataset, String, Key>& binder)
    {
        String result;
        if ((binder.keyFieldCount() > 0) && (binder.fieldCount() > 0)) {
            result = "update " + binder.table() + " set ";
            for (int i = 0; i < binder.fieldCount(); ++i) {
                result += binder.field(i) + " = :" + binder.table() + "_" + binder.field(i);
                if (i < binder.fieldCount() - 1)
                    result += ",";
            }
            result += " where ";
            for (int i = 0; (i < binder.keyFieldCount()); ++i) {
                result += binder.keyField(i) + " = :" + binder.table() + "_" + binder.keyField(i);
                if (i < binder.keyFieldCount() - 1)
                    result += " and ";
            }
        }
        return result;
    }
    /*!
        Generate delete query.
        @param[in] binder DBObjectWrapper to use.        
        @return Query text or empty string in case of error.
     */
    static String deleteQuery(const DBObjectWrapper<Dataset, String, Key>& binder)
    {
        String result;
        if (binder.keyFieldCount() > 0) {
            result = "delete from " + binder.table() + " where ";
            for (int i = 0; (i < binder.keyFieldCount()); ++i) {
                result += binder.keyField(i) + " = :" + binder.table() + "_" + binder.keyField(i);
                if (i < binder.keyFieldCount() - 1)
                    result += " and ";
            }
        }
        return result;
    }    
};

#endif // DBDMLQUERY_H
