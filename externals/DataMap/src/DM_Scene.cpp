#include "DM_Scene.h"
#include "DM_Conversions.h"

#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QGraphicsItemGroup>

namespace DM {
Scene::Scene(QObject * parent) :
	QGraphicsScene(parent)
{
	m_cursor = new QGraphicsTextItem("0, 0"); //Fixed at 0, 0
	addItem(m_cursor);
	m_cursor->setPos(0,0);

	// Initialize all item groups
	for (unsigned int i=0; i<Data::NUM_DT; ++i) {
		m_dataGroup[(Data::DataType)i] = new QGraphicsItemGroup;
		addItem(m_dataGroup[(Data::DataType)i]);
	}

	// Add svg renderer and load map of germany as svg
	QSvgRenderer *renderer = new QSvgRenderer(this);

	// load svg
	renderer->load(QString(":/gfx/Karte_Deutschland.svg"));

	// construct an graphics item and then we set the renderer
	QGraphicsSvgItem *item = new QGraphicsSvgItem();
	item->setSharedRenderer(renderer);
	item->setZValue(-99);
	item->setPos(0,0);
	addItem(item); // add the item to the scene

	QGraphicsEllipseItem *ellipse = addEllipse(0-10,0-10,20,20, QPen(Qt::red), Qt::blue);
	// Hold the pointer
	m_mapSvgItem = item;

	QRectF rectSvgItem = item->sceneBoundingRect();

	addRect(rectSvgItem, QPen(Qt::red));

}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent * event) {

}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

	int height = m_mapSvgItem->boundingRect().size().height();
	int width = m_mapSvgItem->boundingRect().size().width();

	QPointF pointPos = event->scenePos() + m_mapSvgItem->pos();;

	convertPosToCoordinates(pointPos, height, width, m_latitude, m_longitude);

	QString string = QString("%1° N,\n%2° O")
			.arg(m_latitude)
			.arg(m_longitude);

	m_cursor->setPlainText(string);
	m_cursor->setZValue(99); // always on top
	m_cursor->setDefaultTextColor(Qt::black);

	// Set the new position of the cursor
	m_cursor->setPos(event->scenePos().x() + 20, event->scenePos().y() + 20 );

}

void Scene::addDwdDataPoint(const Data::DataType & type, const QString &str, const IBK::Time &minDate, const IBK::Time &maxDate, const double & lat, const double & lon) {
	DataItem *item = new DataItem(m_mapSvgItem->boundingRect(), str, minDate, maxDate, lat, lon, type);
	m_dataGroup[type]->addToGroup(item);

	update();
}

void Scene::setItemGroupVisiblity(const Data::DataType &type, bool visible) {
	m_dataGroup[type]->setVisible(visible);
}


}
