/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_MergedCellsH
#define QtExt_MergedCellsH

#include <QRectF>

namespace QtExt {

	/*! \brief Helper struct for cell merging.*/
	struct MergedCells {
		/*! Default constructor.*/
		MergedCells() :
			firstCol(0),
			firstRow(0),
			lastCol(0),
			lastRow(0),
			m_verticalText(false)
		{}

		/*! Constructor with full argument list.*/
		MergedCells(unsigned int fc, unsigned int fr, unsigned int lc, unsigned int lr) :
				firstCol(fc),
				firstRow(fr),
				lastCol(lc),
				lastRow(lr),
				m_verticalText(false)
		{}

		unsigned int	firstCol;		///< First column.
		unsigned int	firstRow;		///< First row.
		unsigned int	lastCol;		///< last column.
		unsigned int	lastRow;		///< Last row.
		QRectF			m_cellRect;		///< Rectangle of the whole cell area.
		QRectF			m_textRect;		///< Rectangle of the text area.
		QRectF			m_maxTextRect;	///< Maximum possible rectangle for text.
		bool			m_verticalText;	///< Hold if test is vertical or horizontal
	};

} // namespace QtExt

#endif // QtExt_MergedCellsH
