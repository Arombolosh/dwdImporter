#include "DWDDownloader.h"

#include <QMessageBox>

DWDDownloader::DWDDownloader(QObject *parent)
	: QObject(parent)
{
}

void DWDDownloader::append(const QStringList &urls)
{
	for (const QString &urlAsString : urls)
		append(QUrl::fromUserInput(urlAsString));

	if (m_downloadQueue.isEmpty())
		QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DWDDownloader::append(const QUrl &url)
{

	m_downloadQueue.enqueue(url);
	++m_totalCount;

	if (m_downloadQueue.isEmpty()) {
		//startNextDownload();
		QTimer::singleShot(10, this, SLOT(startNextDownload()));
	}

}

QString DWDDownloader::saveFileName(const QUrl &url)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();

	QString filepath = "../../data/Tests/" + basename;

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

void DWDDownloader::startNextDownload()
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

//void DWDDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
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

void DWDDownloader::downloadFinished()
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

void DWDDownloader::downloadReadyRead()
{
	m_output.write(m_currentDownload->readAll());
}

bool DWDDownloader::isHttpRedirect() const
{
	int statusCode = m_currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303
		   || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DWDDownloader::reportRedirect()
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
