#ifndef DM_ConversionsH
#define DM_ConversionsH

#include "DM_Scene.h"
#include "DM_Data.h"

namespace DM {
/*! Converts coordinates to a position at the scene. */
static QPointF convertCoordinatesToPos(QRectF rect, const double &lat, const double &lon) {
	double height = rect.height();
	double width = rect.width();

	QPointF pos;
	pos.setX( height - height * (lat - 47.271679) / ( 55.05864 - 47.271679 ) );
	pos.setY( width  + width  * (15.043611 - lon) / ( 5.866944 - 15.043611 ) );

	return pos;
}

static QColor colorFromDataType(const Data::DataType &dt) {

	/*! Map with colors of data. */
	switch (dt) {
		case Data::DT_TemperatureAndHumidity:
			return QColor("#9B2915");
		case Data::DT_Precipitation:
			return QColor("#c7f9cc");
		case Data::DT_Solar:
			return QColor("#E9B44C");
		case Data::DT_Wind:
			return QColor("#004E98");
		case Data::DT_Pressure:
			return QColor("#1C110A");
	}
	return QColor();
}
}

#endif // DM_ConversionsH
