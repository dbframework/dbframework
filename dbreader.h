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
