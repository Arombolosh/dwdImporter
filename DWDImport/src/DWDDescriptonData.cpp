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

#if 0
DownloadManager::DownloadManager(QObject *parent)
	: QObject(parent)
{
}

void DownloadManager::append(const QStringList &urls)
{
	for (const QString &urlAsString : urls)
		append(QUrl::fromEncoded(urlAsString.toLocal8Bit()));

	if (m_downloadQueue.isEmpty())
		QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DownloadManager::append(const QUrl &url)
{
	if (m_downloadQueue.isEmpty())
		QTimer::singleShot(0, this, SLOT(startNextDownload()));

	m_downloadQueue.enqueue(url);
	++m_totalCount;
}

QString DownloadManager::saveFileName(const QUrl &url)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();

	QString filepath = "../../data/" + basename;

	if (QFile::exists(filepath)) {
		QMessageBox::warning(nullptr,QString(), QString("File delete: '%1' ").arg(filepath));
		QFile::remove(filepath);
	}

#if 0

	if (basename.isEmpty())
		basename = "download";

	if (QFile::exists(basename)) {
		// already exists, don't overwrite
		int i = 0;
		basename += '.';
		while (QFile::exists(basename + QString::number(i)))
			++i;

		basename += QString::number(i);
	}
#endif

	return filepath;
}

void DownloadManager::startNextDownload()
{
	if (m_downloadQueue.isEmpty()) {
		printf("%d/%d files downloaded successfully\n", m_downloadedCount, m_totalCount);
		emit finished();
		return;
	}

	QUrl url = m_downloadQueue.dequeue();

	QString filename = saveFileName(url);
	m_output.setFileName(filename);
	if (!m_output.open(QIODevice::WriteOnly)) {
		fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n",
				qPrintable(filename), url.toEncoded().constData(),
				qPrintable(m_output.errorString()));

		startNextDownload();
		return;                 // skip this download
	}

	QNetworkRequest request(url);
	m_currentDownload = m_manager.get(request);
	connect(m_currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
			SLOT(downloadProgress(qint64,qint64)));
	connect(m_currentDownload, SIGNAL(finished()),
			SLOT(downloadFinished()));
	connect(m_currentDownload, SIGNAL(readyRead()),
			SLOT(downloadReadyRead()));

	// prepare the output
	printf("Downloading %s...\n", url.toEncoded().constData());
	m_downloadTime.start();
}

//void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
//{
//    progressBar.setStatus(bytesReceived, bytesTotal);

//    // calculate the download speed
//    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
//    QString unit;
//    if (speed < 1024) {
//        unit = "bytes/sec";
//    } else if (speed < 1024*1024) {
//        speed /= 1024;
//        unit = "kB/s";
//    } else {
//        speed /= 1024*1024;
//        unit = "MB/s";
//    }

//    progressBar.setMessage(QString::fromLatin1("%1 %2")
//                           .arg(speed, 3, 'f', 1).arg(unit));
//    progressBar.update();
//}

void DownloadManager::downloadFinished()
{
	//progressBar.clear();
	m_output.close();

	if (m_currentDownload->error()) {
		// download failed
		fprintf(stderr, "Failed: %s\n", qPrintable(m_currentDownload->errorString()));
		m_output.remove();
	} else {
		// let's check if it was actually a redirect
		if (isHttpRedirect()) {
			reportRedirect();
			m_output.remove();
		} else {
			printf("Succeeded.\n");
			++m_downloadedCount;
		}
	}

	m_currentDownload->deleteLater();
	startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
	m_output.write(m_currentDownload->readAll());
}

bool DownloadManager::isHttpRedirect() const
{
	int statusCode = m_currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303
		   || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::reportRedirect()
{
	int statusCode = m_currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QUrl requestUrl = m_currentDownload->request().url();
	QTextStream(stderr) << "Request: " << requestUrl.toDisplayString()
						<< " was redirected with code: " << statusCode
						<< '\n';

	QVariant target = m_currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (!target.isValid())
		return;
	QUrl redirectUrl = target.toUrl();
	if (redirectUrl.isRelative())
		redirectUrl = requestUrl.resolved(redirectUrl);
	QTextStream(stderr) << "Redirected to: " << redirectUrl.toDisplayString()
						<< '\n';
}
#endif

void DWDDescriptonData::downloadDescriptionFiles(bool isRecent){
	QString baseDirFTP = "ftp://opendata.dwd.de/climate_environment/CDC/observations_germany/climate/hourly/";

	QString type = "recent/";

	if(!isRecent)
	type = "historical/";

	QStringList sl;
	sl << baseDirFTP + "air_temperature/" + type + "TU_Stundenwerte_Beschreibung_Stationen.txt";
	printf(sl.last().toStdString().c_str());
	sl << baseDirFTP + "solar/ST_Stundenwerte_Beschreibung_Stationen.txt";
	printf(sl.last().toStdString().c_str());
	sl << baseDirFTP + "wind/" + type +"FF_Stundenwerte_Beschreibung_Stationen.txt";
	printf(sl.last().toStdString().c_str());
	sl << baseDirFTP + "pressure/" + type + "P0_Stundenwerte_Beschreibung_Stationen.txt";
	printf(sl.last().toStdString().c_str());

	//DownloadManager manager;
	//manager.append(sl);

	//QObject::connect(&manager, SIGNAL(finished()), this, SLOT(quit()));
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
			break;
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
