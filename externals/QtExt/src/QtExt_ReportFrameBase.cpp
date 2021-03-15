/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_ReportFrameBase.h"

#include "QtExt_Report.h"
#include "QtExt_TextProperties.h"

namespace QtExt {

ReportFrameBase::ReportFrameBase(	Report* report,
									QTextDocument* textDocument ) :
	m_report(report),
	m_textDocument(textDocument)
{
}

} // namespace QtExt {
