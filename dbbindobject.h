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

#ifndef DBBINDOBJECT_H
#define DBBINDOBJECT_H

#include "dbbinder.h"

namespace dbframework {

/*!
    The DBBindObject template class is a base class for implementing DBBinder descendants that bind
    SQL query parameters with the members of some class instance.
    with some class instance data.

    Template parameters.

    Dataset - see DBBinder.

    Object is the class which members are binded with the SQL query parameters.
*/
template <class Dataset, class Object>
class DBBindObject : public DBBinder<Dataset> {
protected:
    /*!
        Object instance which members are binded with SQL query parameters.
     */
    Object* m_object;
public:
    /*!
        Creates DBBindObject instance.
        @param[in] object Pointer to Object instance which members are binded with SQL query parameters.
        The DBBindObject doesn't take ownership of object.
     */
    DBBindObject(Object* object) : DBBinder<Dataset>(), m_object(object) {};
};

}

#endif // DBBINDOBJECT_H
