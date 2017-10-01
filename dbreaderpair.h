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

#ifndef DBREADERPAIR_H
#define DBREADERPAIR_H

#include "dbreader.h"

namespace dbframework {

/*!
    The DBReaderPair template class is the DBReader descendant that is used to read SQL-query results using two
    provided DBReader descendant instances. 

    Template parameters.

    Dataset is the class that provides access to the results of SQL query execution as to the sequence of records.

    Operator is the binary function pointer or functional object that accepts two boolean arguments and returns boolean value.
    Must have copy constructor.
*/
template <class Dataset, typename Operator>
class DBReaderPair : public DBReader<Dataset>{
private:    
    DBReader<Dataset>* m_first;    
    DBReader<Dataset>* m_second;
    Operator m_op;
public:
    /*!
         Creates DBReaderPair instance. Stores pointers to DBReader passed as arguments.
         @param[in] first Pointer to the first DBReader. DBReaderPair doesn't take ownership of first.
         @param[in] second Pointer to the second DBReader. DBReaderPair doesn't take ownership of second.
         @param[in] second Pointer to the second DBReader. DBReaderPair doesn't take ownership of second.
         @param[in] op Binary function that will be called wiith read results as arguments.
    */
    DBReaderPair(DBReader<Dataset>* first, DBReader<Dataset>* second, Operator& op) :
        DBReader<Dataset>(), m_first(first), m_second(second), m_op(op) {};

    /*!
        Implements reading from the dataset. Returns Operator(first->read(ds), second->read(ds)),
        where first and second are pointers to DBReader.
        @param[in] ds Dataset to read from.
        @return The return value of Operator called with results of reading using the first and the second DBReader
        as arguments (Operator(first->read(ds), second->read(ds))).
    */
    bool read(Dataset& ds)
    {
        return m_op(m_first->read(ds), m_second->read(ds));
    };
};

}

#endif // DBREADERPAIR_H
