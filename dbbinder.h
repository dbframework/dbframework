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
