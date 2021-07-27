#ifndef DWDDownloaderH
#define DWDDownloaderH

#include <QtNetwork>
#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QtCore>
#include <QThread>
#include <QProgressBar>


class DWDDownloader: public QObject
{
	Q_OBJECT

public:
	DWDDownloader(QWidget * parent);
	void doDownload(const QUrl &url);
	static QString saveFileName(const QUrl &url);
	bool saveToDisk(const QString &filename, QIODevice *data);
	static bool isHttpRedirect(QNetworkReply *reply);

	QStringList				m_urls;
	qint64					m_bytesReceived;
	qint64					m_bytesTotal;
	bool					m_isRunning = true;
	QProgressBar			*m_progress;

signals:
	void finished();
private:
	QNetworkAccessManager manager;
	QVector<QNetworkReply *> currentDownloads;
public slots:
	void execute();
	void downloadProgress(qint64,qint64);
	void downloadFinished(QNetworkReply *reply);
	void sslErrors(const QList<QSslError> &errors);

};

#endif // DWDDownloaderH
