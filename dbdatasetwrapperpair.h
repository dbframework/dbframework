#ifndef DBDATASETWRAPPERPAIR_H
#define DBDATASETWRAPPERPAIR_H

#include "dbdatasetwrapper.h"

/*!
    The DBDatasetWrapperPair class implement dataset wrapper made of
    two dataset wrappers. It is useful if dataset contains master dataset
    related with one to one relation with two detail datasets. 
*/
template <class Dataset>
class DBDatasetWrapperPair : public DBDatasetWrapper<Dataset>
{
private:
    DBDatasetWrapper<Dataset>* m_w1;
    DBDatasetWrapper<Dataset>* m_w2;
    bool m_own;
public:
    /*!
        Creates dataset wrapper from two dataset wrappers.
        @param[in] wrapper1 First wrapper.
        @param[in] wrapper2 Second wrapper.
        @param[in] own True if DBDatasetWrapperPair must take ownership of
        passed wrappers.
     */
    DBDatasetWrapperPair(DBDatasetWrapper<Dataset>* wrapper1, DBDatasetWrapper<Dataset>* wrapper2, bool own)
        :DBDatasetWrapper<Dataset>(wrapper1->dataset())
    {
        m_w1 = wrapper1;
        m_w2 = wrapper2;
        m_own = own;
    };
    /*!
        Destroys dataset. Frees wrappers, passed in the constructor, if ownership was taken.
    */
    ~DBDatasetWrapperPair()
    {
        if (m_own) {
            delete m_w1;
            delete m_w2;
        }
    };
    /*!
        Calls read() from two wrappers.
        @return True if both wrappers had a success.
     */
    bool read()
    {
        bool r1 = m_w1->read();
        bool r2 = m_w2->read();
        return  r1 && r2;
    };
           
    DBDatasetWrapper<Dataset>* first()
    {
        return m_w1;
    };
    
    DBDatasetWrapper<Dataset>* second()
    {
        return m_w2;
    }
};

#endif // DBDATASETWRAPPERPAIR_H
