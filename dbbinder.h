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

#ifndef DBBINDER_H
#define DBBINDER_H

namespace dbframework {

/*!
    The DBBinder template class provides the abstract interface for classes that
    implement binding of the SQL query parameters with values.  Usually you shouldn't
    inherit from DBBinder directly. It is recommended to inherit from one of its specialized descendants.

    Template parameters.

    Dataset is the class that implements SQL query execution and parameters binding.
*/
template <class Dataset>
class DBBinder {
public:
    /*!
         Constructs DBBinder instance.
     */
    DBBinder(){};
    /*!
        Destroys DBBinder.
    */
    virtual ~DBBinder(){};
    /*!
        This abstract method must be implemented by the descendants. It must perform parameters
        binding using provided Dataset object.
        @param[in] dataset Dataset object which is used to perform parameters binding.
     */
    virtual void bind(Dataset& dataset) = 0;
};

}

#endif // DBBINDER_H
