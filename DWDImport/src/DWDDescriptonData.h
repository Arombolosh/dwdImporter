#ifndef DWDDataDescriptionH
#define DWDDataDescriptionH

#include <IBK_Time.h>
#include <IBK_Path.h>

#include <vector>

#include <QtNetwork>
#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QtCore>

#if 0
class DownloadManager: public QObject
{
	Q_OBJECT
public:
	explicit DownloadManager(QObject *parent = nullptr);

	void append(const QUrl &url);
	void append(const QStringList &urls);
	static QString saveFileName(const QUrl &url);

signals:
	void finished();

private slots:
	void startNextDownload();
	//void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadFinished();
	void downloadReadyRead();

private:
	bool isHttpRedirect() const;
	void reportRedirect();

	QNetworkAccessManager	m_manager;
	QQueue<QUrl>			m_downloadQueue;
	QNetworkReply			*m_currentDownload = nullptr;
	QFile					m_output;
	QTime					m_downloadTime;
	//TextProgressBar progressBar;

	int						m_downloadedCount = 0;
	int						m_totalCount = 0;

};
#endif


class DWDDescriptonData
{
public:

	DWDDescriptonData()
	{
		for (unsigned int i=0; i<NUM_D; ++i)
			m_data[i] = 0;

	};

	/*! Download 4 decription files from dwd ftp server. */
	void downloadDescriptionFiles(bool isRecent=true);

	enum Data{
		D_TemperatureAndHumidity,
		D_Solar,
		D_Wind,
		D_Pressure,
		NUM_D
	};

	void readAllDescriptions(std::map<unsigned int, DWDDescriptonData> &stationDescription);


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
