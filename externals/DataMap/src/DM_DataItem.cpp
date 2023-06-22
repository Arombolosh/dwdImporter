#include "DM_DataItem.h"
#include "DM_Conversions.h"


namespace DM {

DataItem::DataItem(const QRectF rect, const QString & str, const IBK::Time &minDate, const IBK::Time &maxDate,
				   const double & lat, const double & lon, const Data::DataType &type, int radius) {
	m_data = Data(str, minDate, maxDate, lat, lon, type);

	QPointF pos = convertCoordinatesToPos(rect, lat, lon);


	// Erstelle den Stiel des Pins als Rechteck
	QGraphicsLineItem* line1 = new QGraphicsLineItem(-radius/2, -radius/2,  radius/2,  radius/2);
	QGraphicsLineItem* line2 = new QGraphicsLineItem(-radius/2,  radius/2,  radius/2, -radius/2);

	QPen pen(colorFromDataType(type), 2);

	line1->setPen(pen);
	line2->setPen(pen);

	line1->moveBy(pos.x(), pos.y());
	line2->moveBy(pos.x(), pos.y());

	addToGroup(line1);
	addToGroup(line2);

}

void DataItem::moveItem(const QPointF & pos) {
	setPos(pos);
}

}
