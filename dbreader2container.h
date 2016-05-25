#ifndef DBREADER2CONTAINER_H
#define DBREADER2CONTAINER_H

#include "dbreader.h"

namespace dbframework {

template <class Dataset, class Object, class Container, class Reader2Object>
class DBReader2Container : public DBReader<Dataset> {
protected:
    Container* m_data;
public:
    DBReader2Container(Container* container) : DBReader<Dataset>(), m_data(container) {};
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
