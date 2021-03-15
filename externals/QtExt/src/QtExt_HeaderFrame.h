/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_HeaderFrameH
#define QtExt_HeaderFrameH

#include <QCoreApplication>
#include <QPixmap>
#include <QString>

#include "QtExt_Table.h"
#include "QtExt_ReportFrameBase.h"

namespace QtExt {

class Report;

/*! \brief Class for report header.
	Its used for configuration and printing of report header with registration information,
	date and possible user logo.
*/
class HeaderFrame : public ReportFrameBase {
	Q_DECLARE_TR_FUNCTIONS(DisclaimerFrame)
public:
	/*! Standard constructor.*/
	HeaderFrame(	Report* report,
					QTextDocument* textDocument);

	/*! Sets the data in the header. */
	void setHeaderData( const QString& applicationName,
						const QString& registrationMessage,
						const QPixmap& userLogo);

	/*! Updates the header text.
		Header is renered as table.
		\param paintDevice Paint device in order to calculate correct sizes.
		\param width Maximum possible width for drawing.
	*/
	virtual void		update(QPaintDevice* paintDevice, double width);

	/*! Prints the header text frame.*/
	virtual void		print(QPainter * p, const QRectF & frame);

private:
	const int			m_logoMargin;			///< Margin beside user logo
	QtExt::Table		m_table;				///< Table for header text
	QString				m_registrationMessage;	///< Message composed from registration.
	QString				m_appName;				///< Application name to be displayed in header.
	QPixmap				m_userLogo;				///< User logo to be displayed in header.
};

} // namespace QtExt {

#endif // QtExt_HeaderFrameH
