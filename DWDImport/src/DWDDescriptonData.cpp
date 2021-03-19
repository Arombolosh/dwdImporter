#include "DWDDescriptonData.h"

#include <IBK_FileReader.h>

#include <QtNetwork>
#include <QMessageBox>
#include <QObject>
/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QTextStream>

#include <cstdio>

#if 1

#endif

QStringList DWDDescriptonData::downloadDescriptionFiles(bool isRecent){
	QString baseDirFTP = "ftp://opendata.dwd.de/climate_environment/CDC/observations_germany/climate/hourly/";

	QString type = "recent/";

	if(!isRecent)
	type = "historical/";

	QStringList sl;
	sl << baseDirFTP + "air_temperature/" + type + "TU_Stundenwerte_Beschreibung_Stationen.txt";
	sl << baseDirFTP + "solar/ST_Stundenwerte_Beschreibung_Stationen.txt";
	sl << baseDirFTP + "wind/" + type +"FF_Stundenwerte_Beschreibung_Stationen.txt";
	sl << baseDirFTP + "pressure/" + type + "P0_Stundenwerte_Beschreibung_Stationen.txt";

	return sl;
}

void DWDDescriptonData::readAllDescriptions(std::map<unsigned int, DWDDescriptonData> &stationDescription){

	IBK::Path filepath("../../data/Tests/TU_Stundenwerte_Beschreibung_Stationen.txt");
	readDescription(filepath, stationDescription, D_TemperatureAndHumidity);

	filepath= IBK::Path ("../../data/Tests/P0_Stundenwerte_Beschreibung_Stationen.txt");
	readDescription(filepath, stationDescription, D_Pressure);

	filepath= IBK::Path ("../../data/Tests/ST_Stundenwerte_Beschreibung_Stationen.txt");
	readDescription(filepath, stationDescription, D_Solar);

	filepath= IBK::Path ("../../data/Tests/FF_Stundenwerte_Beschreibung_Stationen.txt");
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
		DWDDescriptonData dwd;
		try {
			//extract all informations
			dwd.m_id = IBK::string2val<unsigned int>(line.substr(0,5));
			dwd.m_data[dataType] = 1;

			dwd.m_startDate.set( IBK::string2val<unsigned int>(line.substr(6,4)), IBK::string2val<unsigned int>(line.substr(10,2))-1,IBK::string2val<unsigned int>(line.substr(12,2))-1,0);
			dwd.m_endDate.set( IBK::string2val<unsigned int>(line.substr(15,4)), IBK::string2val<unsigned int>(line.substr(19,2))-1,IBK::string2val<unsigned int>(line.substr(21,2))-1,0);
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
			if(stationDescription.find(dwd.m_id) != stationDescription.end())
				stationDescription[dwd.m_id].m_data[dataType] = 1;
			else
				stationDescription[dwd.m_id] = dwd;
		}  catch (IBK::Exception &ex) {
			QMessageBox::warning(nullptr, QString(), QString("Got an exception while reading lines. In line %1\n").arg(i)
								 + QString("%1\n").arg(dwd.m_id)
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_startDate.toDateTimeFormat()))
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_endDate.toDateTimeFormat()))
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
