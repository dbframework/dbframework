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
