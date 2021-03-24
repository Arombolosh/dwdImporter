# remember to set DYLD_FALLBACK_LIBRARY_PATH on MacOSX
# set LD_LIBRARY_PATH on Linux

TARGET = DWDImport
TEMPLATE = app

# this pri must be sourced from all our applications
include( ../../../externals/IBK/projects/Qt/IBK.pri )

QT += xml core gui network printsupport widgets
CONFIG += console

LIBS += -L../../../lib$${DIR_PREFIX} \
        #-lIBKMK \
        -lQtExt \
        -lQuaZIP \
        -lCCM \
        -lTiCPP \
        -lIBK

INCLUDEPATH = \
        ../../src \
        ../../../externals/IBK/src \
        #../../../externals/IBKMK/src \
        ../../../externals/QtExt/src \
        ../../../externals/CCM/src \
        ../../../externals/TiCPP/src \
        ../../../externals/QuaZIP/src

#TRANSLATIONS += ../../resources/translations/GenBuiGui.ts
CODECFORSRC = UTF-8

RESOURCES += \
        ../../resources/DWDImport.qrc \
        ../../resources/DWDImport.qrc

SOURCES += \
        ../../src/Constants.cpp \
        ../../src/DWDData.cpp \
        ../../src/DWDDelegate.cpp \
        ../../src/DWDDescriptonData.cpp \
        #../../src/DWD_CheckBox.cpp \
        ../../src/DWDDownloader.cpp \
        ../../src/DWDMap.cpp \
        ../../src/MainWindow.cpp \
        ../../src/main.cpp

FORMS += \
        ../../src/DWDMap.ui \
        ../../src/MainWindow.ui

HEADERS += \
        ../../src/Constants.h \
        ../../src/DWDData.h \
        ../../src/DWDDelegate.h \
        ../../src/DWDDescriptonData.h \
        #../../src/DWD_CheckBox.h \
        ../../src/DWDDownloader.h \
        ../../src/DWDMap.h \
        ../../src/DWDScene.h \
        ../../src/MainWindow.h

DISTFILES +=


