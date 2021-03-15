/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_PushButton.h"

#include <QAction>
#include <QMouseEvent>


namespace QtExt {


PushButton::PushButton(QWidget * parent ) :
	QPushButton( parent )
{
	setFlat(true);
	setCheckable(true);
	setAutoDefault(false);
	// todo icons
}

void PushButton::mousePressEvent ( QMouseEvent * e ) {
	if (e->button() & Qt::LeftButton && e->modifiers() & Qt::ControlModifier) {
		emit controlClicked();
		e->accept();
	}
	else
		QPushButton::mousePressEvent(e);
}

} // namespace QtExt {
