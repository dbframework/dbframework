#ifndef DBBINDERS_H
#define DBBINDERS_H

#include "dbbinder.h"

namespace dbframework {

/*!
    The DBBinders class is the db reader that binds parameters using nested db binders.

    Template parameters.

    Dataset is the class that provides execution of a SQL query or a stored procedure.
    It is supposed that it can have parameters and supports binding of parameters with
    values.
*/
template <class Dataset>
class DBBinders : public DBBinder<Dataset> {
public:
    /*!
        Type of the pointer to DBBinder<Dataset>.
     */
    typedef DBBinder<Dataset>* BinderPtr;
private:    
    BinderPtr* m_binders;
    bool m_own;
    bool m_ownBinders;
public:
    /*!
        Creates db binder with nested binders passed as the array of pointers.
        @param[in] binders Array of pointers to binders. Last element in the array must be nullptr.
        The DBBinders doesn't take ownership of binders.
    */
    DBBinders(BinderPtr* binders) :
        DBBinder<Dataset>(), m_binders(binders), m_own(false), m_ownBinders(false) {};
    /*!
        Creates db binder with 2 nested binders.
        @param[in] binder1 Pointer to the first binder.
        @param[in] binder2 Pointer to the second binder.
        @param[in] own If true DBBinders takes ownership of passed binders.
     */
    DBBinders(BinderPtr binder1, BinderPtr binder2, bool own = true) : DBBinder<Dataset>()
    {
         m_binders = new BinderPtr[3];
         m_binders[0] = binder1;
         m_binders[1] = binder2;
         m_binders[2] = nullptr;
         m_own = true;
         m_ownBinders = own;
    };
    /*!
        Creates db binder with 3 nested binders.
        @param[in] binder1 Pointer to the first binder.
        @param[in] binder2 Pointer to the second binder.
        @param[in] binder3 Pointer to the third binder.
        @param[in] own If true DBBinders takes ownership of passed binders.
     */
    DBBinders(BinderPtr binder1, BinderPtr binder2, BinderPtr binder3, bool own = true) : DBBinder<Dataset>()
    {
         m_binders = new BinderPtr[4];
         m_binders[0] = binder1;
         m_binders[1] = binder2;
         m_binders[2] = binder3;
         m_binders[3] = nullptr;
         m_own = true;
         m_ownBinders = own;
    };
    /*!
        Creates db binder with 4 nested binders.
        @param[in] binder1 Pointer to the first binder.
        @param[in] binder2 Pointer to the second binder.
        @param[in] binder3 Pointer to the third binder.
        @param[in] binder4 Pointer to the fourth binder.
        @param[in] own If true DBBinders takes ownership of passed binders.
     */
    DBBinders(BinderPtr binder1, BinderPtr binder2, BinderPtr binder3, BinderPtr binder4, bool own = true) : DBBinder<Dataset>()
    {
         m_binders = new BinderPtr[5];
         m_binders[0] = binder1;
         m_binders[1] = binder2;
         m_binders[2] = binder3;
         m_binders[3] = binder4;
         m_binders[4] = nullptr;
         m_own = true;
         m_ownBinders = own;
    };
    /*!
        Destroys db binder.
      */
    ~DBBinders()
    {
        if (m_binders != nullptr) {
            if (m_ownBinders)
            for (BinderPtr* p = m_binders; *p != nullptr; ++p) {
                delete *p;
            }
            if (m_own) delete[] m_binders;
        }
    }
    /*!
        Binds parameters of dataset by calling bind(Dataset &dataset) method of all nested datasets.
        @param[in] dataset Dataset which parameters to bind.
    */
    void bind(Dataset &dataset)
    {
        if (m_binders != nullptr)
            for (BinderPtr* p = m_binders; *p != nullptr; ++p) {
                (*p)->bind(dataset);
            }
    };
};

}

#endif // DBBINDERS_H
