#ifndef DWDDataDescriptionH
#define DWDDataDescriptionH

#include <IBK_Time.h>
#include <vector>

#include <QDate>

class DWDDescriptonData
{
public:

	DWDDescriptonData()
	{
		for (unsigned int i=0; i<NUM_D; ++i)
			m_data[i] = 0;

	};

	enum Data{
		D_TemperatureAndHumidity,
		D_Solar,
		D_Wind,
		D_Pressure,
		NUM_D
	};

	/*! Station id. */
	unsigned int				m_id;

	/*! Start date. */
	IBK::Time					m_startDate;

	/*! End date. */
	IBK::Time					m_endDate;

	/*! Station height in m. */
	double						m_height;

	/*! Station latitude in Deg. */
	double						m_latitude;

	/*! Station longitude in Deg. */
	double						m_longitude;

	/*! Station name. */
	std::string					m_name;

	/*! Station country. */
	std::string					m_country;

	unsigned int				m_data[NUM_D];
	/*

 Stations_id von_datum bis_datum Stationshoehe geoBreite geoLaenge Stationsname Bundesland
----------- --------- --------- ------------- --------- --------- ----------------------------------------- ----------
00003 19500401 20110331            202     50.7827    6.0941 Aachen                                   Nordrhein-Westfalen
00044 20070401 20210314             44     52.9336    8.2370 Großenkneten                             Niedersachsen

	*/
};

#endif // DWDDataDescriptionH
