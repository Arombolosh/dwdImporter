# -------------------------
# Project for QtExt library
# -------------------------

TARGET = QtExt
TEMPLATE = lib

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../../../IBK/projects/Qt/IBK.pri )


QT += gui core network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


unix|mac {
	VER_MAJ = 1
	VER_MIN = 1
	VER_PAT = 0
	VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}
}

LIBS += -lIBK

INCLUDEPATH += ../../src/ \
		../../../IBK/src

DEPENDPATH = $${INCLUDEPATH}

TRANSLATIONS += ../../resources/translations/QtExt_de.ts \
	../../resources/translations/QtExt_fr.ts \
	../../resources/translations/QtExt_it.ts \
	../../resources/translations/QtExt_es.ts \
	../../resources/translations/QtExt_pl.ts \
	../../resources/translations/QtExt_cs.ts \
	../../resources/translations/QtExt_ru.ts

SOURCES += \
	../../src/QtExt_ActiveLabel.cpp \
	../../src/QtExt_AutoUpdateDialog.cpp \
	../../src/QtExt_AutoUpdater.cpp \
	../../src/QtExt_BrowseFilenameWidget.cpp \
	../../src/QtExt_ChartScene.cpp \
	../../src/QtExt_ColorButton.cpp \
	../../src/QtExt_ColorEditWidget.cpp \
	../../src/QtExt_ColorListModel.cpp \
	../../src/QtExt_Constants.cpp \
	../../src/QtExt_DatabaseReference.cpp \
	../../src/QtExt_DoubleWidgetBase.cpp \
	../../src/QtExt_FeatureWidget.cpp \
	../../src/QtExt_FileSelectionWidget.cpp \
	../../src/QtExt_FooterFrame.cpp \
	../../src/QtExt_GraphicsRectItemWithHatch.cpp \
	../../src/QtExt_HeaderFrame.cpp \
	../../src/QtExt_IconButton.cpp \
	../../src/QtExt_LanguageHandler.cpp \
	../../src/QtExt_LanguageStringEditWidget1.cpp \
	../../src/QtExt_LanguageStringEditWidget3.cpp \
	../../src/QtExt_Locale.cpp \
	../../src/QtExt_NorthPanel.cpp \
	../../src/QtExt_PainterSaver.cpp \
	../../src/QtExt_PanelBase.cpp \
	../../src/QtExt_PanelButton.cpp \
	../../src/QtExt_ParameterEdit.cpp \
	../../src/QtExt_PushButton.cpp \
	../../src/QtExt_RectHatchingFunctions.cpp \
	../../src/QtExt_Report.cpp \
	../../src/QtExt_ReportFrameBase.cpp \
	../../src/QtExt_ReportUtilities.cpp \
	../../src/QtExt_RichTextEditWidget.cpp \
	../../src/QtExt_Settings.cpp \
	../../src/QtExt_Splitter.cpp \
	../../src/QtExt_SplitWidget.cpp \
	../../src/QtExt_Style.cpp \
	../../src/QtExt_TableCell.cpp \
	../../src/QtExt_Table.cpp \
	../../src/QtExt_TableWidget.cpp \
	../../src/QtExt_TextEditorExtended.cpp \
	../../src/QtExt_TextFrame.cpp \
	../../src/QtExt_TextFrameInformations.cpp \
	../../src/QtExt_TextProperties.cpp \
	../../src/QtExt_Utf8LineEdit.cpp \
	../../src/QtExt_ValidatingInputBase.cpp \
	../../src/QtExt_ValidatingLineEdit.cpp \
	../../src/QtExt_ValueInputComboBox.cpp \
	../../src/QtExt_ViewBase.cpp \
	../../src/QtExt_AspectRatioBitmapLabel.cpp \
	../../src/QtExt_Directories.cpp \
	../../src/QtExt_ConstructionViewWidget.cpp \
	../../src/QtExt_ConstructionView.cpp \
	../../src/QtExt_ConstructionGraphicsScene.cpp \
    ../../src/QtExt_ConstructionLayer.cpp \
    ../../src/QtExt_MaterialCategory.cpp \
    ../../src/QtExt_MaterialDatabaseSelectionWidget.cpp \
    ../../src/QtExt_MaterialTableModel.cpp \
    ../../src/QtExt_MaterialTableProxyModel.cpp \
    ../../src/QtExt_MaterialDBItemDelegate.cpp \
    ../../src/QtExt_StackedWidgetResizable.cpp \
    ../../src/QtExt_TextFrameWidgetBase.cpp \
    ../../src/QtExt_CoordinateIndexEdit.cpp \
    ../../src/QtExt_ComboBoxCountries.cpp \
    ../../src/QtExt_FileTreeSelector.cpp \
    ../../src/QtExt_TreeWidget.cpp


HEADERS += \
	../../src/QtExt_ActiveLabel.h \
	../../src/QtExt_AutoUpdateDialog.h \
	../../src/QtExt_AutoUpdater.h \
	../../src/QtExt_BrowseFilenameWidget.h \
	../../src/QtExt_CellSizeFormater.h \
	../../src/QtExt_ChartScene.h \
	../../src/QtExt_ColorButton.h \
	../../src/QtExt_ColorEditWidget.h \
	../../src/QtExt_ColorListModel.h \
	../../src/QtExt_Constants.h \
	../../src/QtExt_DatabaseReference.h \
	../../src/QtExt_DoubleWidgetBase.h \
	../../src/QtExt_FeatureWidget.h \
	../../src/QtExt_FileSelectionWidget.h \
	../../src/QtExt_FooterFrame.h \
	../../src/QtExt_GraphicsRectItemWithHatch.h \
	../../src/QtExt_HeaderFrame.h \
	../../src/QtExt_IconButton.h \
	../../src/QtExt_LanguageHandler.h \
	../../src/QtExt_LanguageStringEditWidget1.h \
	../../src/QtExt_LanguageStringEditWidget3.h \
	../../src/QtExt_LineProperties.h \
	../../src/QtExt_Locale.h \
	../../src/QtExt_MergedCells.h \
	../../src/QtExt_NorthPanel.h \
	../../src/QtExt_PainterSaver.h \
	../../src/QtExt_PanelBase.h \
	../../src/QtExt_PanelButton.h \
	../../src/QtExt_ParameterEdit.h \
	../../src/QtExt_PushButton.h \
	../../src/QtExt_RectHatchingFunctions.h \
	../../src/QtExt_ReportFrameBase.h \
	../../src/QtExt_Report.h \
	../../src/QtExt_ReportUtilities.h \
	../../src/QtExt_RichTextEditWidget.h \
	../../src/QtExt_Settings.h \
	../../src/QtExt_Splitter.h \
	../../src/QtExt_SplitWidget.h \
	../../src/QtExt_Style.h \
	../../src/QtExt_TableCell.h \
	../../src/QtExt_Table.h \
	../../src/QtExt_TableWidget.h \
	../../src/QtExt_TextEditorExtended.h \
	../../src/QtExt_TextFrame.h \
	../../src/QtExt_TextFrameInformations.h \
	../../src/QtExt_TextProperties.h \
	../../src/QtExt_Utf8LineEdit.h \
	../../src/QtExt_ValidatingInputBase.h \
	../../src/QtExt_ValidatingLineEdit.h \
	../../src/QtExt_ValueInputComboBox.h \
	../../src/QtExt_varianthelper.h \
	../../src/QtExt_ViewBase.h \
	../../src/QtExt_AspectRatioBitmapLabel.h \
	../../src/QtExt_Directories.h \
	../../src/QtExt_ConstructionViewWidget.h \
	../../src/QtExt_ConstructionView.h \
	../../src/QtExt_ConstructionGraphicsScene.h \
    ../../src/QtExt_ConstructionLayer.h \
    ../../src/QtExt_MaterialCategory.h \
    ../../src/QtExt_MaterialDatabaseSelectionWidget.h \
    ../../src/QtExt_MaterialBase.h \
    ../../src/QtExt_MaterialTableModel.h \
    ../../src/QtExt_MaterialTableProxyModel.h \
    ../../src/QtExt_MaterialDBItemDelegate.h \
    ../../src/QtExt_StackedWidgetResizable.h \
    ../../src/QtExt_TextFrameWidgetBase.h \
    ../../src/QtExt_CoordinateIndexEdit.h \
    ../../src/QtExt_ComboBoxCountries.h \
    ../../src/QtExt_FileTreeSelector.h \
    ../../src/QtExt_TreeWidget.h

OTHER_FILES += \
	../../doc/mainpage.txt

RESOURCES += \
	../../resources/gfx/QtExt.qrc

FORMS += \
	../../src/QtExt_AutoUpdateDialog.ui \
	../../src/QtExt_ConstructionViewWidget.ui \
	../../src/QtExt_FileSelectionWidget.ui \
	../../src/QtExt_LanguageStringEditWidget1.ui \
	../../src/QtExt_LanguageStringEditWidget3.ui \
    ../../src/QtExt_MaterialDatabaseSelectionWidget.ui \
    ../../src/QtExt_CoordinateIndexEdit.ui
