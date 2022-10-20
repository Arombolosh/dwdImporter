#ifndef DM_DataH
#define DM_DataH

#include <QObject>

#include <IBK_Time.h>>

namespace DM {

class Data
{


public:

	enum DataType{
		DT_TemperatureAndHumidity,
		DT_Solar,
		DT_Wind,
		DT_Pressure,
		DT_Precipitation,
		NUM_DT
	};

	/*! Constructor. */
	Data();

	/*! Constructor. */
	Data(const QString &str, const IBK::Time &minDate, const IBK::Time &maxDate,
		const double &lat, const double &lon, const DataType &type):
		m_name(str),
		m_minDate(minDate),
		m_maxDate(maxDate),
		m_latitude(lat),
		m_longitude(lon),
		m_type(type)
	{}

	/*! Unique identification name of data point instance. */
	QString						m_name;

	/*! Minimum Date where Data starts. */
	IBK::Time					m_minDate;

	/*! Maximum Date where Data ends. */
	IBK::Time					m_maxDate;

	/*! Latitude of Data Point. */
	double						m_latitude = 0.0;
	/*! Longitude of Data Point. */
	double						m_longitude = 0.0;

	/*! Data type of item. */
	DataType					m_type = NUM_DT;
};

}

#endif // DM_DataH
