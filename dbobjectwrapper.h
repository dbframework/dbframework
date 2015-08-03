#ifndef DBOBJECTWRAPPER_H
#define DBOBJECTWRAPPER_H

/*!
   Base class for implementing object wrappers. Object wrapper is a class
   assosiated with some other application's class (refered later as data class).
   Object wrapper is intended to provide information about how data class objects are stored and
   binds parameters for sql queries with data class fields.
*/
template<class Dataset, class String, class Key>
class DBObjectWrapper
{
public:
    /*!
       Create object.
    */
    DBObjectWrapper(){};
    /*!
       Destroy object.
    */
    virtual ~DBObjectWrapper(){};
    /*!
       This method shoud bind query params using it's internal data.
       @param[in, out] query Query object to bind params for. On exit it's
       params should be binded.
    */
    virtual void bindParams(Dataset& query) = 0;
    /*!
        This method must return the name of the table where data class objects
        are stored.
        @return Table name.
     */
    virtual const String& table() const = 0;
    /*!
        This method must return the number of key fields in the table where data class objects
        are stored. Key fields are the fields that form primary key of the table.
        @return Key fields count.
     */
    virtual int keyFieldCount() const = 0;
    /*!
        This method must return the number of non-key fields (see keyFieldCount()) in the table 
        where data class objects are stored.
        @return Non-key fields count.
     */
    virtual int fieldCount() const = 0;
    /*!
        This method must return the mame of a key field in the table where data class objects
        are stored. Key fields are the fields that form primary key of the table. Field is identified
        by index.
        @param[in] index Field index, 0 <= index <= keyFieldCount()
        @return Field name if index is valid, empty string otherwise.
     */
    virtual const String& keyField(int index) const = 0;
    /*!
        This method must return the mame of a non-key field in the table where data class objects
        are stored. Key fields are the fields that form primary key of the table. Field is identified
        by index.
        @param[in] index Field index, 0 <= index <= fieldCount()
        @return Field name if index is valid, empty string otherwise.
     */
    virtual const String& field(int index) const = 0; 
    virtual bool read(Dataset& ) = 0;
    virtual bool readKey(Dataset& ds, Key& k) = 0;
};


#endif // DBOBJECTWRAPPER_H
