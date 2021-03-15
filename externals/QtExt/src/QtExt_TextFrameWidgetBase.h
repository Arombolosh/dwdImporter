#ifndef QtExt_TextFrameWidgetBaseH
#define QtExt_TextFrameWidgetBaseH

#include <memory>

#include <QObject>
#include <QWidget>
#include <QTextDocument>

class QPaintDevice;
class QPainter;

namespace QtExt {

class TextFrameWidgetBase : public QWidget
{
	Q_OBJECT
public:
	explicit TextFrameWidgetBase(QWidget *parent = nullptr);

	/*! Destructor.
		Implemented because this class is a abstract base class.
	*/
	virtual ~TextFrameWidgetBase() {}

	/*! Renders the complete frame with the given painter.
		\param p Painter for drawing.
		\param frame External frame for drawing (maximum width, height and position).
	*/
	virtual void print(QPainter * p, const QRectF & frame) = 0;

protected:
	/*! Overrides paint event.*/
	virtual void paintEvent ( QPaintEvent * event );

signals:

protected:
	std::unique_ptr<QTextDocument>	m_textDocument;			///< Pointer to text document object responsible for rendering text and images.
};

} // namespace QtExt

#endif // QtExt_TextFrameWidgetBaseH
