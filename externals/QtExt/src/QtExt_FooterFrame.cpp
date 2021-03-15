/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_FooterFrame.h"

#include <QPainter>
#include <QResource>

#include "QtExt_PainterSaver.h"

#include "QtExt_Report.h"

namespace QtExt {

FooterFrame::FooterFrame(Report* report, QTextDocument* textDocument) :
	ReportFrameBase(report, textDocument),
	m_page(0),
	m_headerFooterFont( report->m_textProperties.normalFont().family(), 10 ),
	m_footerTable(textDocument)
{
}

void FooterFrame::setFooterData(const QString & projectID)
{
	m_projectID = projectID;
}

void FooterFrame::update(QPaintDevice* paintDevice, double width) {

	// Footerframe holds a table with 3 columns:
	// - left holds date
	// - middle holds project ID
	// - right holds page number

	// paint registration informations
	m_footerTable.clear();
	m_footerTable.setTableSize(QSize(width, m_report->m_effectivePageSize.height()));
	m_footerTable.setColumnsRows(3,1);
	m_footerTable.setOuterFrameWidth(0);
	m_footerTable.setInnerFrameWidth(0);
	m_footerTable.setSpacing(0);
	m_footerTable.setBackgroundColor(Qt::white,true);

	// show date
	QString dateString = QString("<small>%1</small>").arg(QDate::currentDate().toString(Qt::SystemLocaleShortDate));
	// bottom left holds date string
	m_footerTable.setCellText(0,0,dateString);
	m_footerTable.cell(0,0).setAlignment(Qt::AlignLeft);

	// print project ID in center
	m_footerTable.cell(1,0).setAlignment(Qt::AlignHCenter);
	m_footerTable.setCellText(1,0,QString("<small>%1</small>").arg(m_projectID));

	// page count on right, but only set during drawing because only then the page count is known

	// draw line at top of table
	m_footerTable.cell(0,0).setBorderWidth(QtExt::TableCell::TopBorder, m_report->m_innerTableFrameWidth);
	m_footerTable.cell(1,0).setBorderWidth(QtExt::TableCell::TopBorder, m_report->m_innerTableFrameWidth);
	m_footerTable.cell(2,0).setBorderWidth(QtExt::TableCell::TopBorder, m_report->m_innerTableFrameWidth);
	m_footerTable.cell(0,0).setTopMargin( 1 ); // 1 mm space between text and top line
	m_footerTable.cell(1,0).setTopMargin( 1 ); // 1 mm space between text and top line
	m_footerTable.cell(2,0).setTopMargin( 1 ); // 1 mm space between text and top line

	// determine size needed by table itself
	m_wholeFrameRect = m_footerTable.tableRect(paintDevice, m_report->m_effectivePageSize.width()).toRect();
	double tableHeight = m_wholeFrameRect.height();
	// add one line as empty space above table (to content)
	tableHeight += m_report->m_textProperties.normal().height;
	m_wholeFrameRect.moveTop(m_report->m_effectivePageSize.height() - tableHeight);
	m_wholeFrameRect.moveLeft(0);

//	// 5 mm space between content frame and footer line
//	double h = 5 * m_report->m_scale; // 5 mm
//	// estimate height of footer
//	QFontMetrics fm(m_headerFooterFont, paintDevice);

//	// 2 lines of text
//	h += 2*fm.lineSpacing();
//	m_wholeFrameRect = QRect(0, static_cast<int>(m_report->m_effectivePageSize.height()-h+1),
//						 m_report->m_effectivePageSize.width(), static_cast<int>(h));
}


void FooterFrame::print(QPainter * p, const QRectF & frame) {

//	QtExt::PainterSaver psave(p);
//	p->setPen(Qt::black);
//	p->setFont(m_headerFooterFont);
//	// first draw footer line 4 mm from top
//	int lineY = static_cast<int>(frame.top() + 4 * m_report->m_scale);
//	p->drawLine(0, lineY, frame.width(), lineY);

//	QRectF tempFrame(frame);
//	tempFrame.setTop(frame.top() + 5 * m_report->m_scale);
//	tempFrame.setRight(frame.right());


//	// first print project/construction specifications
//	QTextOption opt(Qt::AlignLeft);
//	opt.setWrapMode(QTextOption::WordWrap);
//	p->drawText(tempFrame, m_projectID, opt);

	// now draw page number if enabled
	if ( m_report->m_showPageNumbers ) {
		QString pageNumText = tr("<small>Page %1 of %2</small>").arg(m_page).arg(m_report->m_pageCount);
		m_footerTable.cell(2,0).setAlignment(Qt::AlignRight);
		m_footerTable.setCellText(2,0,pageNumText);
	}

	QRectF tableFrame(frame);
	// move frame for table one line down
	double lineHeight = m_report->m_textProperties.normal().height;
	tableFrame.adjust(0,lineHeight,0,lineHeight);
	m_footerTable.drawTable(p, tableFrame.topLeft());

}

} // namespace QtExt {

