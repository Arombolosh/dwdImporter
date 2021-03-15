/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_Utf8LineEdit.h"

namespace QtExt {

Utf8LineEdit::Utf8LineEdit(QWidget *parent) :
	QLineEdit(parent)
{
}


void Utf8LineEdit::setUtf8Text(const std::string & utf8str) {
	setText( QString::fromUtf8( utf8str.c_str() ) );
}


std::string Utf8LineEdit::utf8Text(bool trimmed) const {
	if (trimmed)
		return text().trimmed().toUtf8().data();
	else
		return text().toUtf8().data();
}

} // namespace QtExt
