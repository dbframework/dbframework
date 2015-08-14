#ifndef DBDATASETWRAPPERPAIR_H
#define DBDATASETWRAPPERPAIR_H

#include "dbdatasetwrapper.h"

/*!
    The DBDatasetWrapperPair is a class template implementing dataset wrapper made of
    two dataset wrappers. DBDatasetWrapperPair::read() performes read from both wrappers,
    used when creating DBDatasetWrapperPair instance.
    
    Template parameters.
    Dataset - class implementing functionality of presenting data, retrieved by SQL
    query or stored procedure call, as a container of records supporting forward record
    iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
    
    Usage scenario.
    DBDatasetWrapperPair is used when reading from queries, made by joining master table
    with several detail tables. For example, let query join Master table with tables 
    Detail1, Detail2. Let Detail1Wrapper and Detail2Wrapper be dataset wrappers for tables
    Detail1 and Detail2. Then DBMasterWrapper descendant for Master table can implement
    detailWrapper(...) as follows:
    DBDatasetWrapper* MasterWrapper::detailWrapper(Object& obj) 
    {
        return new DBDatasetWrapperPair<SomeDataset>(new Detail1Wrapper(...), new Detail2Wrapper(...), true);
    }
    By nesting DBDatasetWrapperPair instances it is possible to join as many tables as required.
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
        @param[in] own True if DBDatasetWrapperPair shoud take ownership of wrapper1 and wrapper2.        
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
    /*!
        Gets first wrapper.        
        @return Pointer to first wrapper.
    */
    DBDatasetWrapper<Dataset>* first()
    {
        return m_w1;
    };
    /*!
        Gets second wrapper.        
        @return Pointer to second wrapper.
    */
    DBDatasetWrapper<Dataset>* second()
    {
        return m_w2;
    }
};

#endif // DBDATASETWRAPPERPAIR_H
