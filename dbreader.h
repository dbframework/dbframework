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

#ifndef DBREADER_H
#define DBREADER_H

namespace dbframework {

/*!
    The DBReader template class provides the abstract interface for classes that read SQL query execution result.
    This is the base class for all such classes in dbframework. Usually you shouldn't inherit from DBReader directly.
    It is recommended to inherit from one of its specialized descendants.

    Template parameters.

    Dataset is the class that provides access to the results of SQL query execution as to the sequence of records.
*/
template <class Dataset>
class DBReader {
public:
    /*!
         Default constructor. Although it does nothing in current version, it is recommended to call this constructor
         in the descendants.
    */
    DBReader(){};
    /*!
         Virtual destructor.
    */
    virtual ~DBReader(){};
    /*!
        This abstract method must be implemented by the descendants. It must read data from the current record of the
        provided Dataset and return true in the case of success.
        @param[in] ds Dataset to read from.
        @return True if success.
    */
    virtual bool read(Dataset& ds) = 0;
};

}

#endif // DBREADER_H
