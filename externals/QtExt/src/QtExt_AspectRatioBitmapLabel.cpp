#include "QtExt_AspectRatioBitmapLabel.h"

namespace QtExt {

AspectRatioBitmapLabel::AspectRatioBitmapLabel(QWidget *parent) :
	QLabel(parent)
{
	setScaledContents(false);
	setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}


void AspectRatioBitmapLabel::setPixmap(const QPixmap & pixmap) {
	m_pixmap = pixmap;
	m_aspect = 1;
	if (!m_pixmap.isNull()) {
		QLabel::setPixmap(scaledPixmap());
		if (m_pixmap.width() > 0)
			m_aspect = ((qreal)m_pixmap.height())/m_pixmap.width();
	}
}


int AspectRatioBitmapLabel::heightForWidth( int width ) const {
	return m_pixmap.isNull() ? this->height() : (m_aspect*width);
}


QSize AspectRatioBitmapLabel::sizeHint() const {
	int w = this->width();
	return QSize(w, heightForWidth(w) );
}


QPixmap AspectRatioBitmapLabel::scaledPixmap() const {
	return m_pixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}


void AspectRatioBitmapLabel::resizeEvent(QResizeEvent * /*e*/) {
	if (!m_pixmap.isNull())
		QLabel::setPixmap(scaledPixmap());
}

} // namespace QtExt
