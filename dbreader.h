#ifndef DBREADER_H
#define DBREADER_H

namespace dbframework {

template <class Dataset>
class DBReader {
public:
    DBReader(){};
    virtual ~DBReader(){};
    virtual bool read(Dataset& ds) = 0;
};

}

#endif // DBREADER_H
