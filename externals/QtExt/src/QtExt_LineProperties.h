/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_LinePropertiesH
#define QtExt_LinePropertiesH

#include <QLineF>

namespace QtExt {

	/*! \brief Helper struct for table lines.*/
	struct LineProperties {
		/*! Default constructor.*/
		LineProperties(QLineF line = QLineF()) :
			m_line(line),
			m_width(1.0)
		{}

		QLineF	m_line;		///< Line itself
		qreal	m_width;	///< Line width
	};

} // namespace QtExt

#endif // QtExt_LinePropertiesH
