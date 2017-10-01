/*
Copyright (c) 2015-2017 Sidorov Dmitry

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  
*/ 

/*!
\file dbframework.h
\mainpage 
Introduction
========

dbframework is a small C++ framework, designed for implementing object-relational mapping (ORM) in applications that interact with relational DBMS. Unlike many ORM implementations, dbframework doesn't implement automatic creation of database tables. Main purpose of this framework is to simplify creation of code for reading execution results of SQL queries that join several tables into complex hierarchical data structures. Here complex hierarchical data structure means class with members that are containers, which contain instances of other classes that in their turn have container members etc.

dbframework features
=======================

1. dbframework is implemented as C++ class templates. dbframework is used by inheriting from  or using its class templates. So no additional preprocessors or compilers required.
2. Modification of existing classes is not required.
3. dbframework doesn't contain code for working with some specific DBMS. It is assumed that some external library will be used for executing SQL queries.

Installation
=========

To install dbframework just unpack it to some directory.

Usage
=============

To use dbframework in your application it is necessary to:
- include dbframework.h;
- add dbframework installation directory to the list of include directories.

All framework classes are in dbframework namespace.

The example of using dbframework can be located in example subdirectory.

Short description
================

dbframework is designed to implement interaction beetwen  the main part of the program and the part that is responcible for interaction with relational DBMS. The class from the main part of the program which data is stored in some database table will be called simple class. The class that has members that are simple classes or containers of classes will be called complex class. Complex classes can be used to store results of execution of SQL queries joining several tables.

dbframework containes two main class temlate hierarchys: 
- classes for reading SQL query execution results (basic class template is dbframework::DBReader); 
- classes for binding parameters of SQL query (basic class template is dbframework::DBBinder).
Let's call Reader dbframework::DBReader descendants and Binder - dbframework::DBBinder descendants.

Readers are used in the following way. Before reading record from SQL query execution result Reader instance is linked with the instance of simple or complex class. Then Reader's method read(...) is called. This method must be implemented to perform reading of the record data into the linked object. A result of repetition of this procedure for all records is the reading of the whole result of SQL query execution.

Binders are used in the following way. Before SQL query execution Binder instance is linked with the instance of some class or with variable of standard type (for example, int). For cases when parameters are contained in several variables or objects, dbframework has Binder that allows to combine several Binders to one. Then Binders method bind(...) is called. It should bind object fields or variable with SQL query parameters. After that the SQL query can be executed.

ORM implementation using dbframework can be divided into 4 stages.

1. Creation of the subroutine that controls SQL query execution. This subroutine must have following parameters: SQL query, Binder and Reader. The subroutine must bind SQL query parameters by calling Binder.bind(...), execute SQL query and call Reader.read(...) for each record of query execution result. Later the execution of every SQL query can be done by this subroutine by providing suitable Binder and Reader. Easiest way to do this is to inherit from dbframework::DBSQLExecutor.
2. Creation of the Binder for each simple class. Easiest way to do this is to inherit from template class dbframework::DBBindObject. This stage is optional. If it isn't supposed to execute SQL queries with many parameters binded with the fields of some object, then one can use class templates dbframework::DBBind and dbframework::DBBinders.  
3. Creation of the Reader for each simple class. Such Reader must store data into simple class members. This Reader can be implemented by inheriting from dbframework::DBReader2Object.
4. Selection of the complex class for storing data for each SQL query. Creation of the Reader for selected complex class. Such Reader can be implemented by combining Readers for simple classes and dbframework Reader class templates for storing data into containers.

In this way, the SQL query execution is reduced to creation of suitable Reader and Binder.

The example subdirectory contains the example of application using dbframework. 
*/

#ifndef DBFRAMEWORK_H
#define DBFRAMEWORK_H

#include "dbreader.h"
#include "dbread2object.h"
#include "dbreader2stlcontainer.h"
#include "dbreader2stlcontainerptr.h"
#include "dbreader2stlassociative.h"
#include "dbreader2stlassociativeptr.h"
#include "dbreader2indexedstlcontainerptr.h"
#include "dbbinder.h"
#include "dbbind.h"
#include "dbbindobject.h"
#include "dbbinders.h"
#include "dbreaderpair.h"
#include "dbsqlgeneratorimpl.h"
#include "dbsqlexec.h"
#include "dbobjectdescriptorimpl.h"

namespace dbframework {

/*!    
    dbframework major version.
*/
#define VERSION_MAJOR 1
/*!
    dbframework minor version.
*/
#define VERSION_MINOR 1

}

#endif //DBFRAMEWORK_H
