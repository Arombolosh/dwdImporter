/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_Utf8LineEditH
#define QtExt_Utf8LineEditH

#include <QLineEdit>

namespace QtExt {

/*! Line edit with support for decoding from utf8 std::string and encoding to utf8 std::string. */
class Utf8LineEdit : public QLineEdit {
	Q_OBJECT
public:
	explicit Utf8LineEdit(QWidget *parent = 0);

	/*! Sets the text from the string which is expected to be UTF-8 encoded. */
	void setUtf8Text(const std::string & utf8str);
	/*! Returns the text from the line edit as string encoded in UTF-8.
		\param trimmed If true, the string is also trimmed, whitespaces removed at front and back.
	*/
	std::string utf8Text(bool trimmed=true) const;
	/*! Implemented if users really want to access the QString in the line edit. */
	QString originalText() const { return QLineEdit::text(); }
	/*! Implemented if users really want to set the content with a QString argument. */
	void setOriginalText(const QString & s) { QLineEdit::setText(s); }

private:
	/*! Implemented as private to disallow users to call this function. */
	void setText(const QString & s) { QLineEdit::setText(s); }
	/*! Implemented as private to disallow users to call this function. */
	QString text() const { return QLineEdit::text(); }
};

} // namespace QtExt

#endif // QtExt_Utf8LineEditH
