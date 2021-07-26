#ifndef DWDDataDescriptionH
#define DWDDataDescriptionH

#include <IBK_Time.h>
#include <IBK_Path.h>

#include <vector>

class QStringList;


class DWDDescriptonData
{
public:

	DWDDescriptonData()
	{
		for (unsigned int i=0; i<NUM_D; ++i)
			m_data[i] = 0;

	};

	/*! Download 4 decription files from dwd ftp server. */
	QStringList downloadDescriptionFiles(bool isRecent=true);

	enum Data{
		D_TemperatureAndHumidity,
		D_Solar,
		D_Wind,
		D_Pressure,
		NUM_D
	};
	/*! Read predefined (download dwd) description files. */
	void readAllDescriptions(std::map<unsigned int, DWDDescriptonData> &stationDescription);


	/*! Station id. */
	unsigned int				m_id;

	/*! Start date of data type. */
	IBK::Time					m_startDate[NUM_D];

	/*! End date of data type. */
	IBK::Time					m_endDate[NUM_D];

	/*! End date string.*/
	std::string					m_endDateString;

	/*! Start date string.*/
	std::string					m_startDateString;


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

	/*! data container. */
	unsigned int				m_data[NUM_D];

	/*! show database entry in table */
	bool						m_isVisible;

private:
	void readDescription(const IBK::Path &filepath, std::map<unsigned int, DWDDescriptonData> &stationDescription, const Data &dataType);

	/*

 Stations_id von_datum bis_datum Stationshoehe geoBreite geoLaenge Stationsname Bundesland
----------- --------- --------- ------------- --------- --------- ----------------------------------------- ----------
00003 19500401 20110331            202     50.7827    6.0941 Aachen                                   Nordrhein-Westfalen
00044 20070401 20210314             44     52.9336    8.2370 Großenkneten                             Niedersachsen

	*/
};

#endif // DWDDataDescriptionH
