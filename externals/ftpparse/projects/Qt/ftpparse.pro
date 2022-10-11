# ----------------------------------------
# Project for ftpparse lib
# Includes only those components that
# are needed by IntegratorFramework
# ----------------------------------------

TARGET  = ftpparse
TEMPLATE = lib

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../../../IBK/projects/Qt/IBK.pri )

unix|mac {
	VER_MAJ = 2
	VER_MIN = 7
	VER_PAT = 0
	VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}
}

HEADERS += \
	../../src/ftpparse.h

SOURCES += \
	../../src/ftpparse.c
