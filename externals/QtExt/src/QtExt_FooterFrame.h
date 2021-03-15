/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_FooterFrameH
#define QtExt_FooterFrameH

#include <QCoreApplication>
#include <QDate>

#include "QtExt_Table.h"
#include "QtExt_TextFrame.h"
#include "QtExt_ReportFrameBase.h"

namespace QtExt {

class Report;

/*! \brief Class for report footer.
	Its used for configuration and printing of report footer with registration information and page numbers.
*/
class FooterFrame : public ReportFrameBase
{
	Q_DECLARE_TR_FUNCTIONS(FooterFrame)
public:
	/*! Standard constructor.*/
	FooterFrame(Report* report, QTextDocument* textDocument);

	/*! Prepares frame for drawing.
		\param paintDevice Paint device in order to calculate correct sizes.
		\param width Maximum possible width for drawing.
	*/
	virtual void		update(QPaintDevice* paintDevice, double width);

	/*! Renders the complete frame with the given painter.
		\param p Painter for drawing.
		\param frame External frame for drawing (maximum width, height and position).
		Call setPage before call this function in order to have the correct page number.
	*/
	virtual void		print(QPainter * p, const QRectF & frame);

	/*! Set the page number used for printing.
		\param page Current page number
	*/
	void setPage(int page) { m_page = page; }

	/*! Sets the common content for the page Footers.
		Texts are expected to be plain text, however, additional html tags are simply forwarded.
	*/
	void setFooterData(const QString& projectID);

private:
	int					m_page;					///< Current page.
	QFont				m_headerFooterFont;		///< Font used for footer.
	QtExt::Table		m_footerTable;			///< Table for footer text
	QString				m_projectID;			///< ProjectID
};


} // namespace QtExt {

#endif // QtExt_FooterFrameH
