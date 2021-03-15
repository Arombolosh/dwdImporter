/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_CellSizeFormaterH
#define QtExt_CellSizeFormaterH

#include <QtGlobal>

namespace QtExt {

	/*! \brief Struct CellSizeFormater contains the table cell format option and size.*/
	/*! It is used for row and column size formating.*/
	struct CellSizeFormater {
		/*! Size formating type.*/
		enum FormatType {
							TableWidth,		///< ??
							AutoMinimum,	///< Automatic calculation of minimum cell rect for given text.
							Fixed,			///< Cell size is fixed.
							Unknown			///< Unknown property is used for non valid cell size.
						};

		qreal		m_size;			///< Current size.
		qreal		m_fixedSize;	///< Size for fixed mode.
		FormatType	m_format;		///< Size format type.

		/*! Default constructor.*/
		CellSizeFormater() :
			m_size(0.0),
			m_fixedSize(0.0),
			m_format(TableWidth)
		{}

		/*! Constructor with predefined format.*/
		CellSizeFormater(FormatType format) :
			m_size(0.0),
			m_fixedSize(0.0),
			m_format(format)
		{}
	};

} // namespace QtExt

#endif // QtExt_CellSizeFormaterH
