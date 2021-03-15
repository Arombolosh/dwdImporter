#ifndef QtExt_AspectRatioBitmapLabelH
#define QtExt_AspectRatioBitmapLabelH

#include <QLabel>
#include <QPixmap>

namespace QtExt {

/*! A label that can be assigned a bitmap which is enlarged and shrunk based on the label size, hereby
	keeping its aspect ratio.
*/
class AspectRatioBitmapLabel : public QLabel {
	Q_OBJECT
public:
	AspectRatioBitmapLabel(QWidget *parent = 0);

	void setPixmap(const QPixmap & pixmap);
	/*! Re-implemented to provide a size hint based on a given label width.*/
	virtual int heightForWidth( int width ) const;
	/*! Implementation of a size hint. */
	virtual QSize sizeHint() const;
	/*! Returns the scaled pixmap. */
	QPixmap scaledPixmap() const;

protected:
	void resizeEvent(QResizeEvent *);

private:
	QPixmap m_pixmap;

	/*! Aspect ratio = height/width, updated in setPixmap(). */
	double m_aspect;
};

} // namespace QtExt

#endif // QtExt_AspectRatioBitmapLabelH
