/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_BrowseFilenameWidgetH
#define QtExt_BrowseFilenameWidgetH

#include <QWidget>

class QLineEdit;
class QToolButton;

namespace QtExt {

/*! A widget that provides a line edit for entering a file name/path manually and a browse button that opens
	a file selection dialog to manually select the file.
	\code
	// to select existing files only
	filenameWidget->setup("myfile.txt", true, true, tr("Text files (*.txt);;All files (*.*)"));

	// to existing directories
	filenameWidget->setup("myfile.txt", false, true, QString());

	// to select directories that may not yet exist
	filenameWidget->setup("myfile.txt", false, false, QString());
	\endcode
*/
class BrowseFilenameWidget : public QWidget {
	Q_OBJECT
public:
	explicit BrowseFilenameWidget(QWidget *parent = 0);

	/*! Sets up line edit. */
	void setup(const QString & filename, bool filenameMode, bool fileMustExist, const QString & filter);

	/*! Sets a filename in the line edit. */
	void setFilename(const QString & filename);

	/*! Returns the filename currently held in the line edit. */
	QString filename() const;

	/*! The line edit (to set tab order). */
	QLineEdit		*lineEdit() { return m_lineEdit; }
	/*! The tool button (to set tab order). */
	QToolButton		*toolBtn() { return m_toolBtn; }

	/*! When set to read-only, button is disabled and line edit is made read-only. */
	void setReadOnly(bool readOnly);

signals:
	/*! Emitted when filename in line edit has changed. */
	void editingFinished();

	/*! Emitted, when return was pressed in line edit (to complete the editing of the filename). */
	void returnPressed();

private slots:
	void onToolBtnClicked();

private:
	/*! If true, tool button requests a file, otherwise a directory. */
	bool			m_filenameMode;
	/*! If true, the file must exist when browsing for the file/directory. */
	bool			m_fileMustExist;
	/*! Filter to use in file dialog (only applicable for filename-mode). */
	QString			m_filter;

	/*! The line edit. */
	QLineEdit		*m_lineEdit;
	/*! The tool button. */
	QToolButton		*m_toolBtn;
};

} // namespace QtExt

#endif // QtExt_BrowseFilenameWidgetH
