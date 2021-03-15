#-------------------------------------------------
#
# Project created by QtCreator 2010-10-06T14:00:57
#
#-------------------------------------------------

QT       -= gui

TARGET = CompressTest
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += ../..

TEMPLATE = app


SOURCES += main.cpp
QMAKE_LIBDIR += ../../../../lib
LIBS += -lQtExt

