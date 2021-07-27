TEMPLATE = subdirs

SUBDIRS = \
		IBK \
		#IBKMK \
		QtExt \
		QuaZIP \
		CCM \
		TiCPP \
		qtftp \
		DWDImport

# where to find the sub projects
#DWDImport.file = ../../DWDImport/projects/Qt/DWDImport.pro

IBK.file = ../../externals/IBK/projects/Qt/IBK.pro
#IBKMK.file = ../../externals/IBKMK/projects/Qt/IBKMK.pro
QtExt.file = ../../externals/QtExt/projects/Qt/QtExt.pro
QuaZIP.file = ../../externals/QuaZIP/projects/Qt/QuaZIP.pro
CCM.file = ../../externals/CCM/projects/Qt/CCM.pro
TiCPP.file = ../../externals/TiCPP/projects/Qt/TiCPP.pro
qtftp.file = ../../externals/qftp/projects/Qt/qftp.pro


DWDImport.file = ../../DWDImport/projects/Qt/DWDImport.pro


# dependencies
#IBKMK.depends = IBK
QtExt.depends = IBK
QuaZIP.depends = IBK
CCM.depends = IBK TiCPP
TiCPP.depends = IBK
DWDImport.depends = IBK QtExt qtftp #IBKMK

