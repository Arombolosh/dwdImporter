#include "DM_Scene.h"

#include <QSvgRenderer>
#include <QGraphicsSvgItem>

namespace DM {
Scene::Scene(QObject * parent) :
	QGraphicsScene(parent)
{
	m_cursor = new QGraphicsTextItem("0, 0"); //Fixed at 0, 0
	addItem(m_cursor);
	m_cursor->setPos(0,0);
	m_cursor->setPos(0,0);

	// Initialize all item groups
	for (unsigned int i=0; i<Data::NUM_DT; ++i) {
		m_dataGroup[(Data::DataType)i] = nullptr;
	}

	// Add svg renderer and load map of germany as svg
	QSvgRenderer *renderer = new QSvgRenderer();

	// load svg
	renderer->load(QString(":/gfx/Germany_location_map.svg"));

	// construct an graphics item and then we set the renderer
	QGraphicsSvgItem *item = new QGraphicsSvgItem();
	item->setSharedRenderer(renderer);
	addItem(item); // add the item to the scene

	// Hold the pointer
	m_mapSvgItem = item;

	// fix the scene rect size
	setSceneRect(sceneRect());

}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent * event) {

}

void Scene::addDwdDataPoint(const Data::DataType & type, const QString &str, const double & lat, const double & lon) {
	DataItem *item = new DataItem(m_mapSvgItem->boundingRect(), str, lat, lon, type);
	m_dataGroup[type]->addToGroup(item);

	update();
}


}
