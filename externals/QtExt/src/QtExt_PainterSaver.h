/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_PainterSaverH
#define QtExt_PainterSaverH

#include <QFont>
#include <QPen>

namespace QtExt {

class PainterSaver {
public:
	PainterSaver(QPainter* painter);
	~PainterSaver();
private:
	PainterSaver(const PainterSaver&) {}
	PainterSaver& operator=(const PainterSaver&) { return *this; }
	QPainter*	m_painter;
};

} // namespace QtExt
#endif // QtExt_PainterSaverH
