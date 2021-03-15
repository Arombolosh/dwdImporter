# -------------------------------------------------
# Project created by QtCreator 2010-04-07T11:08:31
# -------------------------------------------------
TARGET = TableTest
TEMPLATE = app
INCLUDEPATH = ..
SOURCES += TableTest.cpp \
	TableTestMain.cpp \
	../QtExt_TableWidget.cpp \
	../QtExt_TableCell.cpp \
	../QtExt_Table.cpp \
	../QtExt_TextFrame.cpp \
	../QtExt_TextFrameInformations.cpp
HEADERS += TableTestMain.h \
	../QtExt_TableWidget.h \
	../QtExt_TableCell.h \
	../QtExt_Table.h \
	../QtExt_TextFrame.h \
	../QtExt_TextFrameInformations.h
FORMS += TableTestMain.ui

# LIBS += -lgcov
QMAKE_CFLAGS += -pg
QMAKE_CXXFLAGS += -pg

# QMAKE_CXXFLAGS+=--coverage
QMAKE_LFLAGS += -pg
# QMAKE_LFLAGS += -a
