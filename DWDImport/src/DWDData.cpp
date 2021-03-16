#include "DWDData.h"

#include <IBK_StringUtils.h>


void DWDData::addDataLine(std::string &line, std::set<DataType> &dataType){
	std::vector<std::string> data;



	//explode line
	IBK::explode(line, data, ";", true);

	if(data.empty())
		return;

	std::set<DataType>::const_iterator it = dataType.begin();
	IBK::Time time;

	//check startdate
	if(!m_startTime.isValid()){
		return;	//time is not valid
	}

	if(data.size()>2){
		try {
			//station code
			unsigned int stationId = IBK::string2val<unsigned int>(data[0]);		//not used yet

			//get timepoint
			std::string& s = data[1];
			unsigned int year	= IBK::string2val<unsigned int>(s.substr(0,4));
			unsigned int month	= IBK::string2val<unsigned int>(s.substr(4,2));
			unsigned int day	= IBK::string2val<unsigned int>(s.substr(6,2));
			unsigned int hour	= IBK::string2val<unsigned int>(s.substr(8,2));
			time.set(year, month, day, hour*3600);
		}  catch (IBK::Exception &ex) {

		}

	}
	else
		return;	//if no time point is given return

	double timepoint = time.secondsUntil(m_startTime)/3600;
	//shift all data because startpoint is later
	if(timepoint<0){
		timepoint *=-1;
		unsigned int newTimepoint = static_cast<unsigned int>(timepoint);
		m_data.resize(newTimepoint + m_data.size());
	}

	//add data to interval data
	while(it!= dataType.end()) {

		unsigned int col = getColumnDWD(*it);
		if(col < data.size()){
			m_data[timepoint].setVal(*it, IBK::string2val<double>(data[col]));
		}
		++it;
	}

	//get max column in data type vector
	for(unsigned int i=1; i<data.size(); ++i){


	}

	//get timepoint


}

unsigned int DWDData::getColumnDWD(const DataType &dt){
	switch (dt) {
		case DT_AirTemperature:					return 3;
		case DT_RelativeHumidity:				return 4;
		case DT_RadiationLongWave:				return 3;
		case DT_RadiationDiffuse:				return 4;
		case DT_RadiationGlobal:				return 5;
		case DT_WindSpeed:						return 3;
		case DT_WindDirection:					return 4;
		case DT_Pressure:						return 4;
	}
	return 0;
}