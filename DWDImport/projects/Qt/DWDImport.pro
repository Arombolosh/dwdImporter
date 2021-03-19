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
	-lIBK

INCLUDEPATH = \
	../../src \
	../../../externals/IBK/src \
	#../../../externals/IBKMK/src \
	../../../externals/QtExt/src

#TRANSLATIONS += ../../resources/translations/GenBuiGui.ts
CODECFORSRC = UTF-8

#RESOURCES += \
#	../../resources/DWDImport.qrc

SOURCES += \
	../../src/Constants.cpp \
	../../src/DWDData.cpp \
	../../src/DWDDescriptonData.cpp \
	#../../src/DWD_CheckBox.cpp \
	../../src/DWDDownloader.cpp \
	../../src/MainWindow.cpp \
	../../src/main.cpp

FORMS += \
	../../src/MainWindow.ui

HEADERS += \
	../../src/Constants.h \
	../../src/DWDData.h \
	../../src/DWDDescriptonData.h \
	#../../src/DWD_CheckBox.h \
	../../src/DWDDownloader.h \
	../../src/MainWindow.h


