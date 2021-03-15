/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_ReportUtilitiesH
#define QtExt_ReportUtilitiesH

#include <QString>

namespace QtExt {

/*! Formating flags for text.*/
enum FormatTags {
	FT_Normal,		///< Normal text.
	FT_Header1,		///< Header first category.
	FT_Header2,		///< Header second category.
	FT_Header3,		///< Header 3rd category.
	FT_Header4,		///< Header 4th category.
	FT_Small		///< Small text.
};

QString htmlCaptionFormat(const QString& text, FormatTags tag);

FormatTags smallerFormatTag(FormatTags org);

FormatTags biggerFormatTag(FormatTags org);

} // namespace QtExt

#endif // QtExt_ReportUtilitiesH
