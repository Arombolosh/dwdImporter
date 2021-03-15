/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_AutoUpdateDialogH
#define QtExt_AutoUpdateDialogH

#include <QDialog>

namespace Ui {
	class AutoUpdateDialog;
}

class QNetworkAccessManager;
class QNetworkReply;

namespace QtExt {

/*! This is the dialog that shows the update information and manages the download
	of the new software version.

	This dialog is created and shown from AutoUpdater and should normally not be
	used directly.
*/
class AutoUpdateDialog : public QDialog {
	Q_OBJECT
public:
	explicit AutoUpdateDialog(QWidget *parent = 0);
	~AutoUpdateDialog();

	/*! Call this function instead of exec(). */
	int run(const QString & currentVersion, const QString & newVersion, const QString & changeLogText,
			const QString & downloadUrl, const QString & localFilePath);


	/*! Set to true, if user rejects update. */
	bool m_updateRejected;

private slots:

	void on_pushButtonDownload_clicked();

	/*! Connected to network reply to update progress bar. */
	void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

	/*! Triggered when download has completed. */
	void onDownloadFinished(QNetworkReply*);

	void on_pushButtonSkipUpdate_clicked();

private:

	Ui::AutoUpdateDialog	*m_ui;

	/*! Download url for update installer, set in run(). */
	QString					m_downloadUrl;
	/*! Path where downloaded file shall be stored in, set in run(). */
	QString					m_localFilePath;


	/*! Network manager, used to execute network requests. */
	QNetworkAccessManager	*m_networkManager;
};

} // namespace QtExt

#endif // QtExt_AutoUpdateDialogH
