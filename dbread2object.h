#ifndef DBREAD2OBJECT_H
#define DBREAD2OBJECT_H

#include "dbreader.h"

namespace dbframework {

/*!
    The DBReader template class provides the abstract interface for classes that read SQL-query execution result.

    The DBReader2Object template class is a special desccendant of DBReader. Inherit from DBReader2Object when
    you implement DBReader descendants that must store SQL-query execution result in the fields of some object.
    Descendants of DBReader2Object must implement DBReader::read method to read data from the current record of the
    SQL-query execution result and store it in the object's fields.
    fields.

    Template parameters.

    Dataset - see DBReader.

    Object is the class which instance is used to store data.
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
        Constructs DBReader2Object not assosiated with the object.
    */
    DBReader2Object() : DBReader<Dataset>(), m_object(nullptr) {};
    /*!
        Constructs DBReader2Object assosiated with the object.
        @param[in] obj Pointer to the object that is used to store read data. The DBReader2Object doesn't take
        ownership of obj.
    */
    DBReader2Object(Object *obj) : DBReader<Dataset>(), m_object(obj) {};
    /*!
        Get pointer to object used to store read data.
        @return Pointer to object used to store read data or nullptr if object wasn't assosiated.
    */
    Object* object() {return m_object;};
    /*!
        Assosiate object used to store read data with DBReader2Object instance.
        @param[in] obj Pointer to object used to store read data. The DBReader2Object doesn't take
        ownership of obj.
    */
    virtual void setObject(Object* obj) {m_object = obj;};
};

}

#endif // DBREAD2OBJECT_H
