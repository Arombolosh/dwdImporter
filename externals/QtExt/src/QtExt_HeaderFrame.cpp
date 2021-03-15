/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_HeaderFrame.h"

#include <QResource>
#include <QPainter>

#include "QtExt_Report.h"

namespace QtExt {

HeaderFrame::HeaderFrame(Report *report, QTextDocument* textDocument):
	ReportFrameBase(report, textDocument),
	m_logoMargin(5),
	m_table(textDocument)
{

}


void HeaderFrame::setHeaderData(const QString& applicationName,
								const QString& registrationMessage,
								const QPixmap& userLogo )
{
	m_appName = applicationName;
	m_registrationMessage = registrationMessage;
	m_userLogo = userLogo;
}


void HeaderFrame::update(QPaintDevice* paintDevice, double width) {

	// Headerframe holds a table with two columns, left holds registration message, right holds user logo (if provided)

	m_table.clear();
	m_table.setTableSize(QSize(width, m_report->m_effectivePageSize.height()));
	m_table.setColumnsRows(2,1);
	m_table.setOuterFrameWidth(0);
	m_table.setInnerFrameWidth(0);
	m_table.setSpacing(0);
	m_table.setBackgroundColor(Qt::white,true);
	m_table.setCellText(0,0,tr("%1").arg(m_appName));

	// if user logo is given, resize row of the table to hold the logo
	if( !m_userLogo.isNull() )
	{
		// determine height of logo and adjust table row height
		qreal res = (qreal)paintDevice->logicalDpiX() / m_userLogo.logicalDpiX();
		m_table.setRowSizeFormat(0, QtExt::CellSizeFormater::Fixed, m_userLogo.height() * res);
	}

	// left column holds reg message
	m_table.setCellText(0,0,m_registrationMessage);

	// draw line at bottom of table
	m_table.cell(0,0).setBorderWidth(QtExt::TableCell::BottomBorder, m_report->m_innerTableFrameWidth);
	m_table.cell(1,0).setBorderWidth(QtExt::TableCell::BottomBorder, m_report->m_innerTableFrameWidth);

	m_table.cell(0,0).setBottomMargin( 1 ); // 1 mm space between text and top line
	m_table.cell(1,0).setBottomMargin( 1 ); // 1 mm space between text and top line

	m_wholeFrameRect = m_table.tableRect(paintDevice, m_report->m_effectivePageSize.width()).toRect();
	m_wholeFrameRect.moveTop(0);
	m_wholeFrameRect.moveLeft(0);
	// add an empty line as space between header-frame line and following text
	m_wholeFrameRect.adjust(0, 0, 0, m_report->m_textProperties.normal().height);
}


void HeaderFrame::print(QPainter * p, const QRectF & frame) {

	m_table.drawTable(p, frame.topLeft());

	if( m_userLogo.isNull() ) // don't do anything if picture is not valid
		return;

	QPointF logoPos = frame.topRight();
	qreal res = p->device()->logicalDpiX() / (qreal)m_userLogo.logicalDpiX();
	double scaledWidth = m_userLogo.width() * res;
	double scaledHeight = m_userLogo.height() * res;

	logoPos.setX( logoPos.x() - scaledWidth);
	logoPos.setY( logoPos.y());
	p->drawPixmap(QRect(logoPos.x(), logoPos.y(), scaledWidth, scaledHeight), m_userLogo);
}

} // namespace QtExt {
