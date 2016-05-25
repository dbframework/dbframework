#ifndef DBREAD2OBJECT_H
#define DBREAD2OBJECT_H

#include "dbreader.h"

namespace dbframework {

template <class Dataset, class Object>
class DBReader2Object : public DBReader<Dataset> {
protected:
    Object *m_object;
public:
    DBReader2Object(Object *obj) : DBReader(), m_object(obj) {};
};

}

#endif // DBREAD2OBJECT_H
