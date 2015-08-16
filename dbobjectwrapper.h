#ifndef DBOBJECTWRAPPER_H
#define DBOBJECTWRAPPER_H

/*!
The DBObjectWrapperAbstract is a base class template for hierarchy of object wrappers. Object wrapper is a class
assosiated with some other application's class (refered later as Object).
Object wrapper is intended to:
- provide information about table structure used to store Object in the database;
- binds parameters for sql queries with Object members;
- read Object from dataset record.
The DBObjectWrapperAbstract is the root of object wrappers  hierarchy of object wrappers and therefore
doesn't has Object as it's template parameter. All object wrappers should have a constructor
taking one argument - pointer to wrapped Object.

Template parameters.
Dataset - class implementing functionality of presenting data, retrieved by SQL
query or stored procedure call, as a container of records supporting forward record
iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
String - class implementing string. It should have default constructor, constructor
String(char* str), assignment operator and operator+ defined as string concatenation.
Key - class implementing object unique identifier. See DBObjectWrapper description for
details.

Usage scenario.
It is not recommended to inherit from DBObjectWrapperAbstract directly. Inherit from DBObjectWrapper,
which contains default implementation of required methods and Object as template parameter. The
DBObjectWrapperAbstract is internded to be an ancestor of all object wrappers for all Objects.
*/
template<class Dataset, class String, class Key>
class DBObjectWrapperAbstract
{
public:    
    /*!
       Destroy object.
    */
    virtual ~DBObjectWrapperAbstract(){};
    /*!
       This method shoud bind query params using Object data.
       @param[in, out] ds Dataset to bind params for. On exit it's
       params should be binded with Object members.
    */
    virtual void bindParams(Dataset& ds) = 0;
    /*!
        This method must return the name of the table where Object is stored.
        @return Table name.
     */
    virtual String table() const = 0;
    /*!
        This method must return the number of key fields in the table where Object
        is stored. Key fields are the fields that form primary key of the table.
        @return Key fields count.
     */
    virtual int keyFieldCount() const = 0;
    /*!
        This method must return the number of non-key fields (see keyFieldCount()) in the table 
        where Object is stored.
        @return Non-key fields count.
     */
    virtual int fieldCount() const = 0;
    /*!
        This method must return the mame of a key field in the table where Object is
        Key fields are the fields that form primary key of the table. Field is identified
        by index.
        @param[in] index Field index, 0 <= index < keyFieldCount()
        @return Field name if index is valid, empty string otherwise.
     */
    virtual String keyField(int index) const = 0;
    /*!
        This method must return the mame of a non-key field in the table where Object is
        stored. Key fields are the fields that form primary key of the table. Field is identified
        by index.
        @param[in] index Field index, 0 <= index < fieldCount()
        @return Field name if index is valid, empty string otherwise.
     */
    virtual String field(int index) const = 0;
    /*!
        This method should read data from the dataset record to the Object members.
        @param[in] ds Dataset to read from.
        @return True if success.
    */
    virtual bool read(Dataset& ds) = 0;
    /*!
        This method must read key fields ffrom the dataset record to the Key object.
        @param[in] ds Dataset to read from.
        @param[out] k Filled with key field values on exit.
        @return True if success.
     */
    virtual bool readKey(Dataset& ds, Key& k) = 0;
};

/*!
The DBObjectWrapper is a base class template for implementing object wrappers.
Object wrapper is a class
assosiated with some other application's class (refered later as Object).
Object wrapper is intended to:
- provide information about table structure used to store Object in the database;
- binds parameters for sql queries with Object members;
- read Object from dataset record.
The DBObjectWrapper is inherited from DBObjectWrapperAbstract. The DBObjectWrapper adds
default implementation of pure virtual methods of DBObjectWrapperAbstract and Object as
a template parameter.

Template parameters.
Dataset - class implementing functionality of presenting data, retrieved by SQL
query or stored procedure call, as a container of records supporting forward record
iteration. Examples of such classes: QSqlQuery in Qt library, TQuery in VCL library.
String - class implementing string. It should have default constructor, constructor
String(char* str), assignment operator and operator+ defined as string concatenation.
Key - class implementing object unique identifier. See DBObjectWrapper description for
details.
Object - class with which wrapper is assosiated.

Usage scenario.
Inherit new object wrapper class from DBObjectWrapper with proper parameters. Implement
methods depending on functionality required from object wrapper. To use object wrapper
with DBDMLQuery implement table(), keyFieldCount(), fieldCount(), keyField(int index),
field(int index). To use object wrapper for binding dataset parameters with Object
members implement bindParams(Dataset& ds). To use object wrapper for reading data using
dataset wrappers implement read(Dataset& ds) and readKey(Dataset& ds, Key& k).
*/
template<class Dataset, class String, class Key, class Object>
class DBObjectWrapper : public DBObjectWrapperAbstract<Dataset, String, Key>
{
private:
    Object* m_object;
public:
    /*!
        Create object wrapper. All descendants must have such constructor with one
        parameter of the type Object*, and call DBObjectWrapper constructor from it.
        @param[in] obj Wrapped object. DBObjectWrapper doesn't take ownership of the obj.
     */
    DBObjectWrapper(Object* obj)
    {
        m_object = obj;
    };
    /*!
        Gets wrapped object.
        @return Pointer to wrapped object.
     */
    Object* object() const
    {
        return m_object;
    }
    /*!
       This method shoud bind query params using Object data. Default implementation do nothing.
       @param[in, out] ds Dataset to bind params for. On exit it's
       params should be binded with Object members.
    */
    void bindParams(Dataset& ds){};
    /*!
        This method must return the name of the table where Object is stored. Default implementation
        turns String().
        @return Table name.
     */
    virtual String table() const
    {
        return String();
    };
    /*!
        This method must return the number of key fields in the table where Object
        is stored. Key fields are the fields that form primary key of the table.
        Default implementation returns 0.
        @return Key fields count.
     */
    virtual int keyFieldCount() const
    {
        return 0;
    };
    /*!
        This method must return the number of non-key fields (see keyFieldCount()) in the table
        where Object is stored. Default implementation returns 0.
        @return Non-key fields count.
     */
    virtual int fieldCount() const
    {
        return 0;
    };
    /*!
        This method must return the mame of a key field in the table where Object is
        Key fields are the fields that form primary key of the table. Field is identified
        by index. Default implementation returns empty string.
        @param[in] index Field index, 0 <= index < keyFieldCount()
        @return Field name if index is valid, empty string otherwise.
     */
    virtual String keyField(int index) const
    {
        return String();
    };
    /*!
        This method must return the mame of a non-key field in the table where Object is
        stored. Key fields are the fields that form primary key of the table. Field is identified
        by index. Default implementation returns empty string.
        @param[in] index Field index, 0 <= index < fieldCount()
        @return Field name if index is valid, empty string otherwise.
     */
    virtual String field(int index) const
    {
        return String();
    };
    /*!
        This method should read data from the dataset record to the Object members.
        Default implementation do nothing and return false.
        @param[in] ds Dataset to read from.
        @return True if success.
    */
    virtual bool read(Dataset& ds)
    {
        return false;
    };
    /*!
        This method must read key fields ffrom the dataset record to the Key object.
        @param[in] ds Dataset to read from. Default implementation do nothing and return false.
        @param[out] k Filled with key field values on exit.
        @return True if success.
     */
    virtual bool readKey(Dataset& ds, Key& k)
    {
        return false;
    };
};
#endif // DBOBJECTWRAPPER_H
