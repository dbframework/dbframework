#-------------------------------------------------
#
# Project created by QtCreator 2016-09-02T10:29:00
#
#-------------------------------------------------

QT       += core
QT += sql

QT       -= gui

TARGET = example
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mydatabase.cpp \
    testtypes.cpp \
    mybinders.cpp \
    myreaders.cpp \
    mydescriptors.cpp


HEADERS += \
    mydatabase.h \
    mydescriptors.h

INCLUDEPATH += ..
