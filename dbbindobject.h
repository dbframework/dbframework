#ifndef DBBINDOBJECT_H
#define DBBINDOBJECT_H

#include "dbbinder.h"

namespace dbframework {

/*!
    The DBBindObject class is a base class for implementing db binders that bind Dataset parameters
    with some class instance data.

    Template parameters.

    Dataset is the class that provides execution of a SQL query or a stored procedure.
    It is supposed that it can have parameters and supports binding of parameters with
    values.

    Object is the class which data is binded with the Dataset parameters.
*/
template <class Dataset, class Object>
class DBBindObject : public DBBinder<Dataset> {
protected:
    /*!
        Object instance which is binded with Dataset parameters.
     */
    Object* m_object;
public:
    /*!
        Creates db binder.
        @param[in] object Object instance which data is binded with Dataset parameters. The DBBindObject
        doesn't take ownership of object.
     */
    DBBindObject(Object* object) : DBBinder<Dataset>(), m_object(object) {};
};

}

#endif // DBBINDOBJECT_H
