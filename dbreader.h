#ifndef DBREADER_H
#define DBREADER_H

namespace dbframework {

/*!
    The DBReader class provides the abstract interface for db reader classes.
    DB reader classes implement the reading of one current record of the Dataset (see template parameters).

    Template parameters.

    Dataset is the class that provides access to the data (possibly retrieved by SQL query or stored procedure).
    It is supposed that the data is organized as some container of records. It is supposed that Dataset provides
    pointer to the current record and some way to navigate between records.
*/
template <class Dataset>
class DBReader {
public:
    /*!
         Constructs an abstract db reader.
    */
    DBReader(){};
    /*!
        Destroys the abstract db reader.
    */
    virtual ~DBReader(){};
    /*!
        Reads data from the current record of the dataset.
        @param[in] ds Dataset to read from.
        @return True if success.
    */
    virtual bool read(Dataset& ds) = 0;
};

}

#endif // DBREADER_H
