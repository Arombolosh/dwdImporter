#include "DWDData.h"

#include <IBK_StringUtils.h>
#include <IBK_FileReader.h>

#include <CCM_SolarRadiationModel.h>
#include <CCM_ClimateDataLoader.h>
#include "Constants.h"

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
			unsigned int month	= IBK::string2val<unsigned int>(s.substr(4,2))-1;
			unsigned int day	= IBK::string2val<unsigned int>(s.substr(6,2));
			unsigned int hour	= IBK::string2val<unsigned int>(s.substr(8,2));
			time.set(year, month, day, hour*3600);
		}  catch (IBK::Exception &ex) {

		}

	}
	else
		return;	//if no time point is given return

	double timepoint = m_startTime.secondsUntil(time)/m_intervalDuration;
	//shift all data because startpoint is later
	unsigned int newTimepoint = static_cast<unsigned int>(timepoint);
	if(timepoint<0){
		//adjust start date
		double timeDiffSec = m_startTime.secondsUntil(time);
		int yearDiff = (int)(std::abs(timeDiffSec) / (8760*60*60)+1);
		m_startTime = IBK::Time(m_startTime.year()-yearDiff,0);
		newTimepoint = m_startTime.secondsUntil(time)/m_intervalDuration;

	}
	//start point is earlier or perfect
	{
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

void DWDData::writeTSV(unsigned int year){
	//first find the start timepoint
	if(m_data.empty())
		return; // no data :(

	// we go through all data and compose a tsv table
	for(unsigned int i=0; i<m_data.size(); ++i){
		// time
		// data
	}
}

void DWDData::exportEPW(unsigned int year, double latitudeDeg, double longitudeDeg) {
	FUNCID(exportEPW);

	CCM::ClimateDataLoader loader;
	//all data is invalid initialized
	loader.initDataWithDefault();
	loader.m_city = "Generic Location";
	loader.m_source = "DWDImporter";
	loader.m_country = "Germany";
	loader.m_timeZone = 1;
	loader.m_elevation = 81;
	loader.m_startYear = year;
	loader.m_latitudeInDegree = latitudeDeg;
	loader.m_longitudeInDegree = longitudeDeg;
	loader.m_comment = "test";

	CCM::SolarRadiationModel solMod;
	solMod.m_climateDataLoader = loader;

	// remove existing data
//	for (int i=0; i<CCM::ClimateDataLoader::NumClimateComponents; ++i) {
//		loader.m_data[i] = std::vector<double>(8760, -99);
//	}

	int idx = m_startTime.secondsUntil(IBK::Time(year,0))/m_intervalDuration;

	for (int i=0; i<8760;++i, ++idx) {
		if(idx > m_data.size() || m_data.empty())
			break;
		if(idx < 0)
			continue;
		///TODO Fehlerbetrachtung muss dann woanders gemacht werden
		IntervalData intVal = m_data[idx];
		loader.m_data[CCM::ClimateDataLoader::Temperature][i] = (intVal.m_airTemp == -999 ? 0 : intVal.m_airTemp);
		loader.m_data[CCM::ClimateDataLoader::RelativeHumidity][i] = intVal.m_relHum < 0 ? 0 : intVal.m_relHum;

		loader.m_data[CCM::ClimateDataLoader::WindDirection][i] = intVal.m_windDirection;
		loader.m_data[CCM::ClimateDataLoader::WindVelocity][i] = intVal.m_windSpeed;

		loader.m_data[CCM::ClimateDataLoader::AirPressure][i] = intVal.m_pressure;

		double dirH = intVal.m_globalRad - intVal.m_diffRad;
		double dirN;
		solMod.convertHorizontalToNormalRadiation(m_intervalDuration*idx, dirH, dirN);

		loader.m_data[CCM::ClimateDataLoader::DirectRadiationNormal][i] = dirN;
		loader.m_data[CCM::ClimateDataLoader::DiffuseRadiationHorizontal][i] = intVal.m_diffRad;
		loader.m_data[CCM::ClimateDataLoader::LongWaveCounterRadiation][i] = intVal.m_counterRad;

	}

	try {
		loader.writeClimateDataEPW(IBK::Path(std::string(DATA_DIR)+ "export.epw"));
	} catch (IBK::Exception &ex) {
		throw IBK::Exception( "Could not write epw file", FUNC_ID);
	}
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

QString DWDData::filename(const DWDData::DataType &type, const QString &numberString, bool isRecent) const{
	QString rec = "_akt";
	if(!isRecent)
		rec = "_hist";

	switch (type) {
	case DT_AirTemperature:
	case DT_RelativeHumidity:
		return "stundenwerte_TU_" + numberString + rec;

	case DT_Pressure:
		return "stundenwerte_P0_" + numberString + rec;

	case DT_WindSpeed:
	case DT_WindDirection:
		return "stundenwerte_FF_" + numberString + rec;

	case DT_RadiationDiffuse:
	case DT_RadiationGlobal:
	case DT_RadiationLongWave:
		return "stundenwerte_ST_" + numberString + rec;
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
