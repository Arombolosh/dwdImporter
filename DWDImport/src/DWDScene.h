#ifndef DWDSCENE_H
#define DWDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

class DWDScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit DWDScene(QObject* parent) : QGraphicsScene(parent) {
		m_cursor = new QGraphicsTextItem("0, 0"); //Fixed at 0, 0
		addItem(m_cursor);
		m_cursor->setPos(0,0);
		m_cursor->setPos(0,0);
	}

	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override 	{

		int width = 589;
		int height = 798;

		m_latitude = 47.271679+(height-event->scenePos().y() )/height*(55.05864-47.271679);
		m_longitude = 15.043611-(width-event->scenePos().x() )/width*( 15.043611-5.866944);

		QString string = QString("%1° N,\n%2° O")
				.arg(m_latitude)
				.arg(m_longitude);

		m_cursor->setPlainText( string );

		// Set the new position of the cursor
		m_cursor->setPos(event->scenePos().x() + 20, event->scenePos().y() + 20 );
	}

	QGraphicsTextItem			*m_cursor;
	double						m_latitude;
	double						m_longitude;
};


#endif // DWDSCENE_H
