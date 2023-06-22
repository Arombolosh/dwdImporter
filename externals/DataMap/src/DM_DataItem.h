#ifndef DM_DATAITEM_H
#define DM_DATAITEM_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPointF>

#include <IBK_Time.h>

#include "DM_Data.h"


namespace DM {

/*! Class for all Graphics Items in Scene. */
class DataItem : public QGraphicsItemGroup
{
public:

	DataItem() {}

	DataItem(const QRectF rect, const QString &str, const IBK::Time &minDate, const IBK::Time &maxDate,
			 const double &lat, const double &lon, const Data::DataType & type, int radius = 10);

	/*! Moves the item to a current position. */
	void moveItem(const QPointF &pos);

	/*! Data of graphics item. */
	Data					m_data;

};

}
#endif // DM_DATAITEM_H
