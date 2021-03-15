/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_AutoUpdateDialog.h"
#include "ui_QtExt_AutoUpdateDialog.h"

#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

#include <IBK_messages.h>
#include <IBK_FormatString.h>

#include "QtExt_Settings.h"

namespace QtExt {

AutoUpdateDialog::AutoUpdateDialog(QWidget *parent) :
	QDialog(parent, Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
	m_updateRejected(false),
	m_ui(new Ui::AutoUpdateDialog),
	m_networkManager(new QNetworkAccessManager(this))
{
	m_ui->setupUi(this);

#if defined(Q_OS_MAC) // Q_OS_UNIX

const char * const FIXED_FONT_FAMILY = "Monaco";

#elif defined(Q_OS_UNIX)

const char * const FIXED_FONT_FAMILY = "Monospace";

#else

const char * const FIXED_FONT_FAMILY = "Consolas";

#endif

	// create push-button for downloading the update
	m_ui->textEditChangeLog->setFontFamily( FIXED_FONT_FAMILY );
	m_ui->textEditChangeLog->setWordWrapMode(QTextOption::NoWrap);
	m_ui->progressBarDownload->setVisible(false);

	connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(onDownloadFinished(QNetworkReply*)));
}


AutoUpdateDialog::~AutoUpdateDialog() {
	delete m_ui;
}


int AutoUpdateDialog::run(const QString & currentVersion, const QString & newVersion, const QString & changeLogText,
						  const QString & downloadUrl, const QString & localFilePath)
{
	m_ui->labelCurrentVersionNumber->setText(currentVersion);
	m_ui->labelNewVersionNumber->setText(newVersion);
	m_ui->textEditChangeLog->setPlainText(changeLogText);
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
	m_localFilePath = QString(localFilePath).replace("version", newVersion);
#else
	m_localFilePath = localFilePath;
#endif
	m_downloadUrl = downloadUrl;
	return exec();
}


void AutoUpdateDialog::on_pushButtonDownload_clicked() {
	IBK::IBK_Message(IBK::FormatString("Downloading update installer to '%1'.").arg(m_localFilePath.toStdString()),
					 IBK::MSG_PROGRESS, "[AutoUpdateDialog::on_pushButtonDownload_clicked]", IBK::VL_STANDARD);
	QUrl addy(m_downloadUrl);
	QNetworkReply * reply = m_networkManager->get( QNetworkRequest(addy) );
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
	m_ui->pushButtonDownload->setEnabled(false);
	m_ui->progressBarDownload->setVisible(true);
	m_ui->progressBarDownload->setValue(0);
}


void AutoUpdateDialog::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
	if (bytesTotal == -1) {
		m_ui->progressBarDownload->setRange(0,0);
		m_ui->progressBarDownload->setValue(0);
	}
	else {
		m_ui->progressBarDownload->setMaximum(bytesTotal);
		m_ui->progressBarDownload->setValue(bytesReceived);
	}
}


void AutoUpdateDialog::onDownloadFinished(QNetworkReply* reply) {
	m_ui->progressBarDownload->setVisible(false);
	m_ui->pushButtonDownload->setEnabled(true);

	if (reply->error() != QNetworkReply::NoError ) {
		QMessageBox::critical(this, tr("Download error"), tr("Couldn't download update installer."));
	}
	else {
		// store downloaded file in selected location
		QFileInfo finfo(m_localFilePath);
		QDir baseDir = finfo.absoluteDir();
		if (!baseDir.exists())
			baseDir.mkpath(baseDir.absolutePath());
		QFile f(m_localFilePath);
		f.open(QIODevice::WriteOnly);
		f.write(reply->readAll());
		f.close();

		// now check if the file was actually written or if it was moved by
		// virus scan software etc.
		if (!QFileInfo::exists(m_localFilePath) || QFileInfo(m_localFilePath).size() == 0) {
			QMessageBox::information(this, tr("Download complete"), tr("The update installer could not be stored "
																	   "on the machine, possibly through access restrictions. "
																	   "Please download and install the update manually."));
		}
		else {

#if defined(Q_OS_WINDOWS)
			QMessageBox::information(this, tr("Download complete"), tr("The software updater has downloaded the update installer (to '%1').\n"
																	   "You must restart the program to apply the update!")
				.arg(m_localFilePath) );

#elif defined(Q_OS_MAC)
			QMessageBox::information(this, tr("Download complete"), tr("The software updater has downloaded the new software package in %1.\n"
																	   "The disk image will now be mounted (you may need to enable third party "
																	   "app downloads in the security settings).")
									 .arg(m_localFilePath) );
			// open DMG
			QString openCommand("open");
			QStringList args  = QStringList() << m_localFilePath;
			QProcess::execute(openCommand, args);
#elif defined(Q_OS_UNIX)
			QMessageBox::information(this, tr("Download complete"), tr("The software updater has downloaded the new software package in %1.")
									 .arg(m_localFilePath) );
#endif
		}
	}

	reply->deleteLater();
}


void AutoUpdateDialog::on_pushButtonSkipUpdate_clicked() {
	int res = QMessageBox::question(this, tr("Skip update"),
									tr("Would you like to skip this update and and not be reminded again for this version?"));
	if (res == QMessageBox::Yes) {
		m_updateRejected = true;
		reject();
	}
}


} // namespace QtExt


