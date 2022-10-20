TEMPLATE = subdirs

SUBDIRS = \
                IBK \
                QtExt \
                QuaZIP \
                CCM \
                TiCPP \
                qtftp \
                qwt \
                DataMap \
                ClimateDataGenerator


# where to find the sub projects
#ClimateDataGenerator.file = ../../ClimateDataGenerator/projects/Qt/ClimateDataGenerator.pro

IBK.file = ../../externals/IBK/projects/Qt/IBK.pro
#IBKMK.file = ../../externals/IBKMK/projects/Qt/IBKMK.pro
QtExt.file = ../../externals/QtExt/projects/Qt/QtExt.pro
QuaZIP.file = ../../externals/QuaZIP/projects/Qt/QuaZIP.pro
CCM.file = ../../externals/CCM/projects/Qt/CCM.pro
TiCPP.file = ../../externals/TiCPP/projects/Qt/TiCPP.pro
qtftp.file = ../../externals/qftp/projects/Qt/qftp.pro
qwt.file = ../../externals/qwt/projects/Qt/qwt.pro
DataMap.file = ../../externals/DataMap/projects/Qt/DataMap.pro

ClimateDataGenerator.file = ../../ClimateDataGenerator/projects/Qt/ClimateDataGenerator.pro


# dependencies
#IBKMK.depends = IBK
QtExt.depends = IBK
QuaZIP.depends = IBK
CCM.depends = IBK TiCPP
TiCPP.depends = IBK
DataMap.depends = IBK QtExt
ClimateDataGenerator.depends = IBK QtExt qtftp qwt DataMap

