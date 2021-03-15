#include "QtExt_TextFrameWidgetBase.h"

#include <QAbstractTextDocumentLayout>
#include <QPainter>

namespace QtExt {

TextFrameWidgetBase::TextFrameWidgetBase(QWidget *parent) :
	QWidget(parent),
	m_textDocument(new QTextDocument)
{

#ifdef Q_OS_WIN
	QFont font;
#else
	QFont font(this->font());
#endif

	// initialise frame
	int iheight = height();
	int iwidth = width();
	QRect frame(10,0,iwidth - 10, iheight);

	// initialise textDocument
	m_textDocument->setDefaultFont(font);
//	m_textDocument->documentLayout()->setPaintDevice(p.device());
}

void TextFrameWidgetBase::paintEvent ( QPaintEvent * event ) {

	QPainter p(this);

#ifdef Q_OS_WIN
	QPalette pal = palette();
	QColor color = pal.color(QPalette::Window);
	p.fillRect(this->rect(), color);
#endif

	// initialise frame
	int iheight = height();
	int iwidth = width();
	QRect frame(10,0,iwidth - 10, iheight);

	print(&p, frame);

	event->accept();
}

} // namespace QtExt
