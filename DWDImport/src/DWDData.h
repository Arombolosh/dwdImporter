#ifndef DWDDataH
#define DWDDataH

#include <map>
#include <vector>
#include <set>

#include <IBK_Time.h>

class DWDData
{
public:

	enum DataType{
		DT_AirTemperature,
		DT_RelativeHumidity,
		DT_RadiationLongWave,
		DT_RadiationDiffuse,
		DT_RadiationGlobal,
		DT_SunElevation,
		DT_WindSpeed,
		DT_WindDirection,
		DT_Pressure,
		NUM_DT
	};

	/*! Add a data line from dwd file.
		map:
		key		-> column
		value	-> which interpretation
	*/
	void addDataLine(std::string &line, std::set<DataType> &dataType);

	struct IntervalData{

		void setVal(const DataType &dt, double val){
			switch (dt) {
				case DT_AirTemperature:						m_airTemp = val;		break;
				case DT_RelativeHumidity:					m_relHum= val;			break;
				case DT_RadiationDiffuse:					m_diffRad= val;			break;
				case DT_RadiationGlobal:					m_globalRad= val;		break;
				case DT_RadiationLongWave:					m_counterRad= val;		break;
				case DT_WindDirection:						m_windDirection= val;	break;
				case DT_WindSpeed:							m_windSpeed= val;		break;
				case DT_Pressure:							m_pressure= val;		break;

			}
		}

		double				m_airTemp		= -999;		///< Air temperature in C
		double				m_relHum		= -999;		///< Relative humidity in %
		double				m_counterRad	= -999;		///< Long wave counter radiation in W/m2
		double				m_diffRad		= -999;		///< Diffuse radiation W/m2
		double				m_globalRad		= -999;		///< Globale radiation W/m2
		double				m_sunElevation	= -999;		///< Sun elavation angle in Deg
		double				m_windSpeed		= -999;		///< Wind speed in m/s
		double				m_windDirection	= -999;		///< Wind direction in Deg
		double				m_pressure		= -999;		///< Pressure in Pa


	};

	IBK::Time					m_startTime;			///< Start time for the first interval data
	unsigned int				m_intervalDuration;		///< Interval duration in sec
	std::vector<IntervalData>	m_data;					///< Vector with interval data

private:
	unsigned int getColumnDWD(const DataType &dt);
};

#endif // DWDDataH