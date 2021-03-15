TEMPLATE = subdirs

SUBDIRS = \
	IBK \
	#IBKMK \
	QtExt \
	DWDImport

# where to find the sub projects
#DWDImport.file = ../../DWDImport/projects/Qt/DWDImport.pro

IBK.file = ../../externals/IBK/projects/Qt/IBK.pro
#IBKMK.file = ../../externals/IBKMK/projects/Qt/IBKMK.pro
QtExt.file = ../../externals/QtExt/projects/Qt/QtExt.pro

DWDImport.file = ../../DWDImport/projects/Qt/DWDImport.pro


# dependencies
#IBKMK.depends = IBK
QtExt.depends = IBK
DWDImport.depends = IBK QtExt #IBKMK

