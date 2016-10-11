#ifndef DBBINDERS_H
#define DBBINDERS_H

#include "dbbinder.h"

namespace dbframework {

/*!
    The DBBinders template class is the DBBinder descendant that uses nested DBBinder
    descendants instances to implement its bind method. The DBBinders template class
    has different constructors, allowing different ways of passing nested DBBinder instances.
    Nested DBBinder instances can be passed to the constructor as an array of pointers or as
    fixed number of pointers (two, three or four). The DBBinders instances, intended for
    binding more parameters, can be constructed by nesting DBBinders instances.

    Template parameters.

    Dataset - see DBBinder.
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
        Creates DBBinders with nested DBBinder instances passed as the array of pointers.
        @param[in] binders Array of pointers to DBBinder. Last element in the array must be nullptr.
        The DBBinders doesn't take ownership of binders.
    */
    DBBinders(BinderPtr* binders) :
        DBBinder<Dataset>(), m_binders(binders), m_own(false), m_ownBinders(false) {};
    /*!
        Creates DBBinders with 2 nested DBBinder instances.
        @param[in] binder1 Pointer to the first instance of DBBinder.
        @param[in] binder2 Pointer to the second instance of DBBinder.
        @param[in] own If true DBBinders takes ownership of passed DBBinder instances.
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
        Creates DBBinders with 3 nested DBBinder instances.
        @param[in] binder1 Pointer to the first instance of DBBinder.
        @param[in] binder2 Pointer to the second instance of DBBinder.
        @param[in] binder3 Pointer to the third instance of DBBinder.
        @param[in] own If true DBBinders takes ownership of passed DBBinder instances.
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
        Creates DBBinders with 4 nested DBBinder instances.
        @param[in] binder1 Pointer to the first instance of DBBinder.
        @param[in] binder2 Pointer to the second instance of DBBinder.
        @param[in] binder3 Pointer to the third instance of DBBinder.
        @param[in] binder4 Pointer to the fourth instance of DBBinder.
        @param[in] own If true DBBinders takes ownership of passed DBBinder instances.
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
        Destroys DBBinders. If DBBinders owns nested DBBinder instances than it deallocates them with call to delete.
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
        Binds parameters of SQL query by calling bind(Dataset &dataset) method of all nested DBBinder instances.
        @param[in] dataset Dataset which is used to perform parameters binding.
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
