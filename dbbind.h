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
