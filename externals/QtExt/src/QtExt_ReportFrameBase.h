/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_ReportFrameBaseH
#define QtExt_ReportFrameBaseH

#include <QRectF>

class QTextDocument;
class QPaintDevice;
class QPainter;

namespace QtExt {

class Report;

/*! Abstract base class for report frames.*/
class ReportFrameBase {
public:
	/*! Only constructor.
		\param report Reference to report (owner).
		\param fontFamily Font family name used for creating standard font.
		\param fontPointSize Fonte size for normal font in points.
		\param textDocument Current textDocument responsible for rendering text. Necessary for derived classes.
	*/
	ReportFrameBase(	Report* report,
						QTextDocument* textDocument);

	/*! Prepares frame for drawing.
		\param paintDevice Paint device in order to calculate correct sizes.
		\param width Maximum possible width for drawing.
	*/
	virtual void update(QPaintDevice* paintDevice, double width) = 0;

	/*! Renders the complete frame with the given painter.
		\param p Painter for drawing.
		\param frame External frame for drawing (maximum width, height and position).
	*/
	virtual void print(QPainter * p, const QRectF & frame) = 0;

	/*! Destructor.
		Implemented because this class is a abstract base class.
	*/
	virtual ~ReportFrameBase() {}

	/*! Returns the whole frame rect necessary for drawing.
		The size of the frame will be calculated by calling update for the given maximum width.
	*/
	QRectF	wholeFrameRect() const { return m_wholeFrameRect; }

	/*! This flag marks if a report shoud be hidden or not in the report manager class. */
	bool			m_isHidden;

protected:
	Report*			m_report;				///< Reference to report.
	QTextDocument*	m_textDocument;			///< Pointer to text document object responsible for rendering text and images.
	QRectF			m_wholeFrameRect;		///< Rectangle of the whole area.

};

} // namespace QtExt {

#endif // QtExt_ReportFrameBaseH
