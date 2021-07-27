# ----------------------------------------
# Project for qftp lib
# Includes only those components that
# are needed by IntegratorFramework
# ----------------------------------------

TARGET  = qftp
TEMPLATE = lib

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../../../IBK/projects/Qt/IBK.pri )

QT += core network

unix|mac {
	VER_MAJ = 0
	VER_MIN = 0
	VER_PAT = 1
	VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}
}

HEADERS += \
	../../src/qftp.h \
	../../src/qurlinfo.h



SOURCES += \
	../../src/qftp.cpp \
	../../src/qurlinfo.cpp

