#ifndef DWDDownloaderH
#define DWDDownloaderH

#include <QtNetwork>
#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QtCore>


class DWDDownloader: public QObject
{
	Q_OBJECT
public:
	explicit DWDDownloader(QObject *parent = nullptr);

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
#endif // DWDDownloaderH
