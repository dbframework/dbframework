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

#ifndef DBBIND_H
#define DBBIND_H

#include "dbbinder.h"

namespace dbframework {

/*!
    The DBBind class is a base class for implementing DBBinder descendants that bind one parameter,
    identified by name, with the value.

    Template parameters.

    Dataset - see DBBinder

    NameType is a type that represents parameter name.

    ValueType is a type that represents parameter value.
*/
template <class Dataset, class NameType, class ValueType>
class DBBind : public DBBinder<Dataset> {
protected:
    /*!
        Parameter name.
     */
    NameType m_name;
    /*!
        Parameter value.
     */
    ValueType m_value;
public:
    /*!
        Constructs DBBind.
        @param[in] name Name of the parameter.
        @param[in] value Value of the parameter.
     */
    DBBind(const NameType& name, const ValueType& value) :
        DBBinder<Dataset>::DBBinder(), m_name(name), m_value(value) {};
};

}


#endif // DBBIND_H
