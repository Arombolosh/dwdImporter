/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_ReportUtilities.h"

namespace QtExt {

QString htmlCaptionFormat(const QString& text, FormatTags level) {
	switch(level) {
		case FT_Small: return QString("<small>%1</small>").arg(text);
		case FT_Normal: return text;
		case FT_Header1: return QString("<h1>%1</h1>").arg(text);
		case FT_Header2: return QString("<h2>%1</h2>").arg(text);
		case FT_Header3: return QString("<h3>%1</h3>").arg(text);
		case FT_Header4: return QString("<h4>%1</h4>").arg(text);
		default: return text;
	}
}

FormatTags smallerFormatTag(FormatTags org) {
	switch(org) {
		case FT_Header1: return FT_Header2;
		case FT_Header2: return FT_Header3;
		case FT_Header3: return FT_Header4;
		case FT_Header4: return FT_Normal;
		case FT_Normal: return FT_Small;
		case FT_Small: return FT_Small;
		default: return FT_Normal;
	}
}

FormatTags biggerFormatTag(FormatTags org) {
	switch(org) {
		case FT_Header1: return FT_Header1;
		case FT_Header2: return FT_Header1;
		case FT_Header3: return FT_Header2;
		case FT_Header4: return FT_Header3;
		case FT_Normal: return FT_Header4;
		case FT_Small: return FT_Normal;
		default: return FT_Normal;
	}
}


} // namespace QtExt
