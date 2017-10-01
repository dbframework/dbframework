#ifndef DBSQLEXEC_H
#define DBSQLEXEC_H

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

#include "dbbinder.h"
#include "dbreader.h"

namespace dbframework {

/*!
    The DBSQLExecutor class template implements the algorithm of retrieving SQL query exectution result
    using DBBinder for parameter binding and DBReader for reading data. Inherit from this class to implement
    this algorithm for a specific Dataset. Descendants must implement executeQuery() and next() methods.

    Template parameters.

    Dataset is the class that provides access to the results of SQL query execution as to the sequence of records.
    It is expected that Dataset has some internal pointer to current record, from which reading is performed, and
    can iterate forward this pointer.
*/
template<class Dataset>
class DBSQLExecutor {
protected:
    /*!
        This method must execute the query using provided Dataset object.
        @param[in] ds Dataset object to use for query execution.
    */
    virtual bool executeQuery(Dataset& ds) = 0;
    /*!
        Must be implemented by descendants. This method is used to iterate forward through the sequence of records retrieved
        as the result of SQL query execution. Upon first call after successful query execution it must make current the first record
        of sequence and return true or return false if the sequence of records is empty. Upon next calls it must make current the 
        next record of sequence and return true or return false if end of sequence reached.
        @param[in] ds Dataset object to use for iteration.
    */
    virtual bool next(Dataset& ds) = 0;
public:
    /*!
        This method binds SQL query parmeters, executes the query and reads the results.
        @param[in] ds Dataset object to use for query execution. It must have SQL query text set and be prepared for parameter
        binding and query execution.
        @param[in] binder Pointer to DBBinder descendant that must be used for parameter binding. If nullptr, then no parameter binding
        is performed. This is useful for SQL queries without parameters. DBSQLExecutor doesn't take ownership of binder.
        @param[in] reader Pointer to DBReader descendant that must be used for reading SQL query execution results. If nullptr, reading is
        not performed. This is useful for queries without result, for example for DML queries. DBSQLExecutor doesn't take ownership of reader.
    */
    bool exec(Dataset& ds, DBBinder<Dataset> *binder, DBReader<Dataset> *reader)
    {
        bool result = true;               

        if (binder != nullptr) {
            binder->bind(ds);
        }

        result = executeQuery(ds);
        
        if (result && (reader != nullptr)) {
            for (;next(ds) && result;) {
                result = reader->read(ds);
            }
        }
       
        return result;
    }
};

}


#endif //DBSQLEXEC_H
