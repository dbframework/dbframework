#ifndef DBREAD2OBJECT_H
#define DBREAD2OBJECT_H

#include "dbreader.h"

namespace dbframework {

/*!
    The DBReader2Object class provides the abstract interface for db reader classes, used to read data to
    sime object.

    Template parameters.

    Dataset is the class that provides access to the data (possibly retrieved by SQL query or stored procedure).
    It is supposed that the data is organized as some container of records. It is supposed that Dataset provides
    pointer to the current record and some way to navigate between records.

    Object is the class used to store data read from the dataset. Object must have default constructor.
*/
template <class Dataset, class Object>
class DBReader2Object : public DBReader<Dataset> {
protected:
    /*!
        Pointer to the object that is used to store read data.
    */
    Object *m_object;
public:
    /*!
        Constructs db reader not assosiated with the object.
    */
    DBReader2Object() : DBReader(), m_object(nullptr) {};
    /*!
        Constructs db reader.
        @param[in] obj Pointer to the object that is used to store read data. The DBReader2Object doesn't take
        ownership of obj.
    */
    DBReader2Object(Object *obj) : DBReader(), m_object(obj) {};
    /*!
        Get pointer to object used to store read data.
        @return Pointer to object used to store read data.
    */
    Object* object() {return m_object;};
    /*!
        Set pointer to object used to store read data.
        @param[in] obj Pointer to object used to store read data.
    */
    virtual void setObject(Object* obj) {m_object = obj;};
};

}

#endif // DBREAD2OBJECT_H
