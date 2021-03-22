#include "DWDData.h"

#include <IBK_StringUtils.h>
#include <IBK_FileReader.h>


void DWDData::createData(const std::map<IBK::Path, std::set<DWDData::DataType>> &filenames, unsigned int intervalDuration) {

	FUNCID(createData);

	m_intervalDuration = intervalDuration;

	for(std::map<IBK::Path, std::set<DWDData::DataType>>::const_iterator	it=filenames.begin();
																			it!=filenames.end(); ++it){
		//check if file exist
		IBK::Path fileName = it->first;
		if(!fileName.exists())
			throw IBK::Exception(IBK::FormatString( "File '%1' does not exist. ").arg(fileName.filename()), FUNC_ID);

		//read the file
		IBK::FileReader fileReader(fileName);
		std::vector<std::string> lines;
		fileReader.readAll(fileName, lines, std::vector<std::string>{"\n"});

		for(unsigned int i=1;i<lines.size(); ++i){
			addDataLine(lines[i], it->second);
		}
	}
}

void DWDData::addDataLine(std::string &line, const std::set<DataType> &dataType){
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

	double timepoint = m_startTime.secondsUntil(time)/3600;
	//shift all data because startpoint is later
	unsigned int newTimepoint = static_cast<unsigned int>(timepoint);
	if(timepoint<0){
		timepoint *=-1;
		std::vector<IntervalData> newData(newTimepoint);
		m_data.insert( m_data.begin(), newData.begin(), newData.end() );
	}
	else{
		if(m_data.size()<=newTimepoint){
			unsigned int aaa =newTimepoint-m_data.size()+1;
			m_data.insert(m_data.end(), aaa, IntervalData());
		}

	}
	//add data to interval data
	while(it!= dataType.end()) {

		unsigned int col = getColumnDWD(*it);

		double unitFactor = 1;
		if(*it == DT_RadiationGlobal || *it == DT_RadiationDiffuse || *it == DT_RadiationLongWave)
			unitFactor = 1/0.36;

		if(col < data.size()){
			m_data[newTimepoint].setVal(*it, IBK::string2val<double>(data[col])*unitFactor);
		}
		++it;
	}

	//get timepoint
}



QString DWDData::urlFilename(const DWDData::DataType &type, const QString &numberString, bool isRecent) const{
	QString rec = "_akt", rec2 = "";
	if(!isRecent)
		rec = "_hist";

	QString base = "ftp://opendata.dwd.de/climate_environment/CDC/observations_germany/climate/hourly/";

	if(!(type == DT_RadiationDiffuse || type == DT_RadiationGlobal || type == DT_RadiationLongWave))
		rec2 = isRecent ? "recent/" : "historical/";

	switch (type) {
	case DT_AirTemperature:
	case DT_RelativeHumidity:
		return base + "air_temperature/" + rec2 + "stundenwerte_TU_" + numberString + rec + ".zip";

	case DT_Pressure:
		return base + "pressure/" + rec2 + "stundenwerte_P0_" + numberString + rec + ".zip";

	case DT_WindSpeed:
	case DT_WindDirection:
		return base + "wind/" + rec2 + "stundenwerte_FF_" + numberString + rec + ".zip";

	case DT_RadiationDiffuse:
	case DT_RadiationGlobal:
	case DT_RadiationLongWave:
		return base + "solar/" + "stundenwerte_ST_" + numberString + rec + ".zip";


	}
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
