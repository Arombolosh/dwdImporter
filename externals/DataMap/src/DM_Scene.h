#ifndef DM_SceneH
#define DM_SceneH

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QWheelEvent>

#include "DM_Data.h"
#include "DM_DataItem.h"

namespace DM {
class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit Scene(QObject* parent);

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

	/*! Adds a data point to the scene. */
	void addDwdDataPoint(const Data::DataType &type, const QString & str, const double &lat, const double &lon);

	/*! Converts coordinates to a position at the scene. */
	QPointF convertCoordinatesToPos(const double &lat, const double &lon);


	QGraphicsTextItem			*m_cursor;

	QGraphicsItem*				m_mapSvgItem = nullptr;
	DataItem*					m_locationItem = nullptr;

	QGraphicsItemGroup			*m_dataGroup[Data::NUM_DT];


private:


	// QGraphicsScene interface
protected:
};
}

#endif // DWDSCENE_H
