#ifndef DBREADER2CONTAINER_H
#define DBREADER2CONTAINER_H

#include "dbreader.h"

namespace dbframework {

/*!
    The DBReader2Container class is a db reader that reads data from the Dataset record to the object and adds
    this object to the container.

    Template parameters.

    Dataset is the class that provides access to the data (possibly retrieved by SQL query or stored procedure).
    It is supposed that the data is organized as some container of records. It is supposed that Dataset provides
    pointer to the current record and some way to navigate between records.

    Object is the class used to store data read from the dataset. Object must have default constructor.

    Container is the class implementing STL-style container of Object. Container must have push_back method
    with one parameter of type Object which adds Object copy to the container.

    Reader2Object must be DBReader2Object<Dataset, Object> descendant. It must have a constructor with one parameter
    of type Object*.
*/
template <class Dataset, class Object, class Container, class Reader2Object>
class DBReader2Container : public DBReader<Dataset> {
protected:
    /*!
        Pointer to the container where read data is stored.
    */
    Container* m_data;
public:
    /*!
        Constructs db reader.
        @param[in] container Pointer to the container that is used to store read data. The DBReader2Container doesn't take
        ownership of container.
    */
    DBReader2Container(Container* container) : DBReader<Dataset>(), m_data(container) {};
    /*!
        Creates instance of Object, reads data from dataset to it using Reader2Object instance and then adds Object instance
        to the container.
        @param[in] ds Dataset to read from.
        @return Returns true.
    */
    bool read(Dataset &ds)
    {
        Object obj;
        Reader2Object reader(&obj);
        if (reader.read(ds)) {
            m_data->push_back(obj);
        }
        return true;
    };
};

};

#endif // DBREADER2CONTAINER_H
