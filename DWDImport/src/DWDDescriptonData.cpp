#include "DWDDescriptonData.h"

#include <IBK_FileReader.h>

#include <QMessageBox>

#include "Constants.h"

QStringList DWDDescriptonData::downloadDescriptionFiles(bool isRecent){
	QString baseDirFTP = "ftp://opendata.dwd.de/climate_environment/CDC/observations_germany/climate/hourly/";

	QString type = "recent/";

	if(!isRecent)
		type = "historical/";

	QStringList sl;
	sl << baseDirFTP + "air_temperature/" + type + "TU_Stundenwerte_Beschreibung_Stationen.txt";
	sl << baseDirFTP + "solar/ST_Stundenwerte_Beschreibung_Stationen.txt";					//no difference between historical or recent
	sl << baseDirFTP + "wind/" + type +"FF_Stundenwerte_Beschreibung_Stationen.txt";
	sl << baseDirFTP + "pressure/" + type + "P0_Stundenwerte_Beschreibung_Stationen.txt";

	return sl;
}

void DWDDescriptonData::readAllDescriptions(std::map<unsigned int, DWDDescriptonData> &stationDescription){

	IBK::Path filepath(std::string(DATA_DIR) + "Tests/TU_Stundenwerte_Beschreibung_Stationen.txt");
	readDescription(filepath, stationDescription, D_TemperatureAndHumidity);

	filepath= IBK::Path (std::string(DATA_DIR) + "Tests/P0_Stundenwerte_Beschreibung_Stationen.txt");
	readDescription(filepath, stationDescription, D_Pressure);

	filepath= IBK::Path (std::string(DATA_DIR) + "Tests/ST_Stundenwerte_Beschreibung_Stationen.txt");
	readDescription(filepath, stationDescription, D_Solar);

	filepath= IBK::Path (std::string(DATA_DIR) + "Tests/FF_Stundenwerte_Beschreibung_Stationen.txt");
	readDescription(filepath, stationDescription, D_Wind);

}

void DWDDescriptonData::readDescription(const IBK::Path &filepath, std::map<unsigned int, DWDDescriptonData> &stationDescription, const Data &dataType){
//	IBK::Path filepath(QtExt::Directories::userDataDir().toStdString() + "filename.txt");

	IBK::FileReader fileReader(filepath);

	//check if file is valid
	if(!fileReader.valid()){
		QMessageBox::warning(nullptr, QString(), QString("File '%1' is not valid").arg(QString::fromStdString(filepath.absolutePath().c_str())));
		return;
	}
	std::vector<std::string> lines;
	//fill lines vector
	fileReader.readAll(filepath, lines,std::vector<std::string>{"\n"});

	//parse each line
	for (unsigned int i=2; i<lines.size(); ++i ) {
		const std::string & line =lines[i];
		if(line.empty())
			continue;
		unsigned int id = IBK::string2val<unsigned int>(line.substr(0,5));
		DWDDescriptonData & dwd = stationDescription[id];

		try {
			//extract all informations
			dwd.m_idStation = id;
			dwd.m_data[dataType] = 1;
//00071 20091201 20191231            759     48.2156    8.9784 Albstadt-Badkap                          Baden-Württemberg
			dwd.m_startDate[dataType].set( IBK::string2val<unsigned int>(line.substr(6,4)),
										   IBK::string2val<unsigned int>(line.substr(10,2))-1,
										   IBK::string2val<unsigned int>(line.substr(12,2))-1,0);
			dwd.m_endDate[dataType].set( IBK::string2val<unsigned int>(line.substr(15,4)),
										 IBK::string2val<unsigned int>(line.substr(19,2))-1,
										 IBK::string2val<unsigned int>(line.substr(21,2))-1,0);
			dwd.m_startDateString = line.substr(6,8);
			dwd.m_endDateString = line.substr(15,8);
			dwd.m_height = IBK::string2val<double>(line.substr(24,38-24));
			dwd.m_latitude = IBK::string2val<double>(line.substr(39,50-39));
			dwd.m_longitude = IBK::string2val<double>(line.substr(51,60-51));
			dwd.m_name = IBK::trim_copy(line.substr(61,100-61));
			dwd.m_country = IBK::trim_copy(line.substr(101,500));

//			QMessageBox::warning(this, QString(),
//								 QString("%1\n").arg(dwd.m_id)
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_startDate.toDateTimeFormat()))
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_endDate.toDateTimeFormat()))
//								 + QString("%1\n").arg(dwd.m_height)
//								 + QString("%1\n").arg(dwd.m_latitude)
//								 + QString("%1\n").arg(dwd.m_longitude)
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_name))
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_country))
//								 + QString("%1\n").arg(QString::fromStdString(line))
//								 );
			//add dwd object to map
			///TODO check if other information are equal

		}  catch (IBK::Exception &ex) {
			QMessageBox::warning(nullptr, QString(), QString("Got an exception while reading lines. In line %1\n").arg(i)
								 + QString("%1\n").arg(dwd.m_idStation)
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_startDate[dataType].toDateTimeFormat()))
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_endDate[dataType].toDateTimeFormat()))
								 + QString("%1\n").arg(dwd.m_height)
								 + QString("%1\n").arg(dwd.m_latitude)
								 + QString("%1\n").arg(dwd.m_longitude)
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_name))
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_country))
								 + QString("%1\n").arg(QString::fromStdString(line))
								 );
		}
	}

}
