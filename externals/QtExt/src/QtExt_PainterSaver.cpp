/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include <QPainter>

#include "QtExt_PainterSaver.h"

namespace QtExt {

PainterSaver::PainterSaver(QPainter* painter): m_painter(painter)
{
	m_painter->save();
}

PainterSaver::~PainterSaver()
{
	m_painter->restore();
}

} // namespace QtExt
