#ifndef DBREADER2ASSOCIATIVE_H
#define DBREADER2ASSOCIATIVE_H

#include "dbreader.h"

namespace dbframework {

template <class Dataset, class Object, class Container, class Reader2Object, class Key, class Reader2Key>
class DBReader2Assosiative : public DBReader<Dataset> {
protected:
    Container* m_data;
public:
    DBReader2Assosiative(Container* data) : DBReader<Dataset>(), m_data(data) {};
    bool read(Dataset& ds)
    {
        Key k;
        Reader2Key keyReader(&k);
        if (keyReader.read(ds)) {
            Reader2Object r(&(*m_data)[k]);
            r.read(ds);
        }
        return true;
    };
};

}

#endif // DBREADER2ASSOCIATIVE_H
