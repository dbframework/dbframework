#ifndef DBBINDER_H
#define DBBINDER_H

namespace dbframework {

/*!
    The DBBinder class provides the abstract interface for db binder classes.
    DB binder classes implement the binding of the SQL query parameters with values.

    Template parameters.

    Dataset is the class that provides execution of a SQL query or a stored procedure.
    It is supposed that it can have parameters and supports binding of parameters with
    values.
*/
template <class Dataset>
class DBBinder {
public:
    /*!
         Constructs an abstract db binder.
     */
    DBBinder(){};
    /*!
        Destroys an abstract db binder.
    */
    virtual ~DBBinder(){};
    /*!
        Binds Dataset parameters with values.
        @param[in] dataset Dataset which parameters are binded.
     */
    virtual void bind(Dataset& dataset) = 0;
};

}

#endif // DBBINDER_H
