#ifndef DM_ConversionsH
#define DM_ConversionsH

#include "DM_Scene.h"
#include "DM_Data.h"


namespace DM {


/*! Converts coordinates to a position at the scene. */
static QPointF convertCoordinatesToPos(QRectF rect, const double &lat, const double &lon) {
	double height = rect.height();
	double width = rect.width();

	double			MIN_LON = 5.866345;
	double			MAX_LON = 15.042245;
	double			MIN_LAT = 47.2705;
	double			MAX_LAT = 55.036572;
	double			DIFF = 0.4;

	QPointF topLeftPoint = rect.topLeft();


	double diffLon = -0.1 + DIFF - 2*DIFF * (lat - MIN_LAT)/(MAX_LAT - MIN_LAT);
	double diffLat = * (lat - MIN_LON)/(MAX_LON - MIN_LON);

	MIN_LON = MIN_LON + diffLon;
	MAX_LON = MAX_LON - diffLon;

	QPointF pos;
	pos.setX( topLeftPoint.x() + width  + width  * (MAX_LON - lon) / ( MIN_LON - MAX_LON ) );
	pos.setY( topLeftPoint.y() + height - height * (lat - MIN_LAT) / ( MAX_LAT - MIN_LAT ) );

	return pos;
}

/*! Converts point in scnene to coordinates. */
static void convertPosToCoordinates(const QPointF pos, const double &mapHeight, const double &mapWidth,
									double &lat, double &lon) {

	double			MIN_LON = 5.866345;
	double			MAX_LON = 15.042245;
	double			MIN_LAT = 47.2705;
	double			MAX_LAT = 55.036572;

	lat = MIN_LAT + ( mapHeight - pos.y() ) / mapHeight * ( MAX_LAT - MIN_LAT );
	lon = MAX_LON - ( mapWidth  - pos.x() ) / mapWidth  * ( MAX_LON - MIN_LON );
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
