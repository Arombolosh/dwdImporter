/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_NorthPanel.h"

#include "QtExt_PushButton.h"
#include "QtExt_IconButton.h"


#include <QPalette>
#include <QColor>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QString>
#include <QButtonGroup>
#include <QProgressBar>
#include <QCoreApplication>
#include <QToolButton>
#include <QDebug>

//#include "QtExt_Constants.h"


namespace QtExt {


NorthPanel::NorthPanel( QWidget * parent) :
	QWidget(parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QPalette pal;
	pal.setColor(QPalette::Background, Qt::white);
	setPalette(pal);

	// The progress widget contains a caption label, a progress bar and a message label
	/// \todo connect abort button to somewhere
	/// \todo add abort button icon

	m_progressWidget = new QWidget(this);
//	m_progressCaption = new QLabel("caption");
//	m_progressMessage = new QLabel("message");
	m_progressAbortButton = new QtExt::IconButton(m_progressWidget);
	m_progressAbortButton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum);
	m_progressAbortButton->setNormalIcons(
		QPixmap(QString::fromUtf8(":/master/delete_16x16/02_delete_inactive.png")),
		QPixmap(QString::fromUtf8(":/master/delete_16x16/03_delete_active.png")),
		QPixmap()
	);
	m_progressAbortButton->setCheckedIcons(
		QPixmap(QString::fromUtf8(":/master/delete_16x16/04_delete_pressed.png")),
		QPixmap(),
		QPixmap()
	);
	connect(m_progressAbortButton, SIGNAL(clicked()), this, SLOT(on_progressAbortButton_clicked()));
	m_progressBar = new QProgressBar();
	m_progressBar->setTextVisible(false);
	m_progressBar->setMaximumHeight(10);
	m_progressBar->setMinimumHeight(10);

	// progress bar and abort button are placed in a horizontal layout
	QHBoxLayout * hlay = new QHBoxLayout;
	hlay->addWidget(m_progressBar,1);
	hlay->addWidget(m_progressAbortButton, 0);
	hlay->setSpacing(2);
	hlay->setContentsMargins(0,0,0,0);

	// caption, message and progress bar + button are placed in a vlayout and set in the
	// progress widget
//	QVBoxLayout * vlay = new QVBoxLayout;
//	vlay->addWidget(m_progressCaption);
//	vlay->addWidget(m_progressMessage);
//	vlay->addLayout( hlay );
//	vlay->setSpacing(2);
//	vlay->setContentsMargins(0,0,0,0);
	m_progressWidget->setLayout(hlay);
	m_progressWidget->setMinimumWidth(250);
	m_progressWidget->setMaximumWidth(250);


	/// \todo customize label and Logo
	m_labelLogo = new QLabel("place UserLogo");
	m_labelLogo->setTextInteractionFlags( Qt::NoTextInteraction );
	m_labelLogo->setPixmap ( QPixmap(QString::fromUtf8(":/master/Delphin_Logo_headline/Version_2/Delphin_Logo_no_background/Delphin_Logo_headline_no_background_50x195.png" ) ) );

	QGridLayout * gridlay = new QGridLayout;
	// logo is placed in the top left (column 0, rows 1 and 2)
	// addWidget (widget, fromRow, fromColumn, rowSpan, columnSpan, Qt::Alignment alignment)
	gridlay->addWidget(m_labelLogo, 0, 0, 2, 1);

	// progress widget is in second column, second row
	gridlay->addWidget(m_progressWidget, 1, 1, 1, 1);

	// create layout for buttons and add outside lines
	m_buttonLayout = new QHBoxLayout;

	// button layout is in row 2, column 3
	gridlay->addLayout(m_buttonLayout, 1, 3);
	// column 2 is stretchable
	gridlay->setColumnStretch(2,1);
	// Margin is off by a few pixels on windows.
	gridlay->setContentsMargins(24,0,18,0);
	setLayout(gridlay);

	m_labelLogo->setVisible( true );
	m_progressWidget->setVisible(false);

	unsigned int npHeight = qMax(50, m_labelLogo->pixmap()->height());
	resize(100, npHeight);
	setMinimumHeight(npHeight);
	setMaximumHeight(npHeight);
}

NorthPanel::~NorthPanel() {
}



void NorthPanel::setLogo( const QPixmap &userLogo ) {

	/// \todo implement
	m_labelLogo->setPixmap ( userLogo );

}




void NorthPanel::addViewButton(QtExt::PushButton * btn) {
	if (m_buttonLayout->count() != 0) {
		QFrame * line = new QFrame;
		line->setFrameShape(QFrame::VLine);
		line->setFrameShadow(QFrame::Sunken);
		m_buttonLayout->addWidget(line);
	}
	m_buttonLayout->addWidget(btn);
}


void NorthPanel::setProgressVisible(bool visible) {
	m_labelLogo->setVisible( (!visible) );
	m_progressWidget->setVisible(visible);
}



void NorthPanel::setProgressCaption(const QString & msg) {
	setProgressVisible(true);
	m_progressBar->setToolTip(msg);
}



void NorthPanel::setProgressInfo(int total, int current, QString ) {

	if (current == -1) {
		// disable progress bar, because we are done
		setProgressVisible(false);
	}
	else if (total != 0) {
		if (!m_progressWidget->isVisible())
			setProgressVisible(true);
		if (m_progressBar->maximum() != total) {
			m_progressBar->setMaximum(total);
		}
		m_progressBar->setValue(current);
		m_progressBar->update();
	}
}



void NorthPanel::on_progressAbortButton_clicked() {
	emit abortClicked();
}


} // namespace QtExt
