#include "DM_DataItem.h"
#include "DM_Conversions.h"


namespace DM {

DataItem::DataItem(const QRectF rect, const QString & str, const IBK::Time &minDate, const IBK::Time &maxDate,
				   const double & lat, const double & lon, const Data::DataType &type, int radius) {
	m_data = Data(str, minDate, maxDate, lat, lon, type);

	QPointF pos = convertCoordinatesToPos(rect, lat, lon);
	setRect(pos.x()-radius/2, pos.y()-radius/2, radius, radius);
	setBrush(colorFromDataType(type));
}

void DataItem::moveItem(const QPointF & pos) {
	setPos(pos);
}

}
