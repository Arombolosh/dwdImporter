#ifndef DM_SceneH
#define DM_SceneH

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QWheelEvent>

#include <IBK_Time.h>

#include "DM_Data.h"
#include "DM_DataItem.h"

namespace DM {
class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit Scene(QObject* parent);


	/*! Adds a data point to the scene. */
	void addDwdDataPoint(const Data::DataType &type, unsigned int id, const QString & str, const IBK::Time &minDate, const IBK::Time &maxDate,
						 const double &lat, const double &lon);

	/*! Converts coordinates to a position at the scene. */
	QPointF convertCoordinatesToPos(const double &lat, const double &lon);

	/*! Sets an item group with all graphics scene items visivle or invisible. */
	void setItemGroupVisiblity(const Data::DataType &type, bool visible);


	QGraphicsTextItem			*m_cursor;

	QGraphicsItem*				m_mapSvgItem = nullptr;
	QGraphicsEllipseItem*		m_locationItem = nullptr;

	DataItem					*m_dataGroup[Data::NUM_DT];

	double						m_longitude;	///< Longitude of mouse position
	double						m_latitude;		///< Latitude of mouse position

	/*! Map with id of weather station to graphics item. */
	std::map<unsigned int, DataItem*> m_idToDataItem;

signals:
	void updatedLocation();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;



private:

};
}

#endif // DWDSCENE_H
