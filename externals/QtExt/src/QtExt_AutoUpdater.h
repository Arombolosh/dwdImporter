/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_AutoUpdaterH
#define QtExt_AutoUpdaterH

#include <QObject>
#include <QMap>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;
class QUrl;

namespace QtExt {

/*! A class that implements all functionality needed for auto-update features.

	Specifically, it implements downloading the update-info file from a given server, extract and compare version
	number and provide the changelog text. Then, it implements downloading the update file and storing it on
	the local machine.
	Finally, it provides the functionality for checking for existing update file, executing it in a separate process
	and marking the update as installed for cleanup during next call.

	Example for use-case 1, start update installer when available:
	\code
	QtExt::AutoUpdater updater;
	if (updater.installUpdateWhenAvailable("/path/to/update_installer.exe"))
		return EXIT_SUCCESS; // close application so that installer can overwrite installed files
	\endcode

	Example for use-case 2, check and display available update info. In this example the auto-updater handles all
	cases:
	* no connection possible (gives out message "Cannot retrieve update information.").
	* no newer version available (gives out message "Your software is up-to-date.").
	* a newer version available (shows the "Update available" dialog. Starts the download of the changelog file and
	  once completed, shows the changelog in the update available dialog. When user clicks on "download", the download
	  of the appropriate installer file is started. Once download is complete, user is prompted to install
	 (starts update with the help of use-case 1).
	\code
	QtExt::AutoUpdater updater;
	updater.checkForUpdateInfo("http://www.bauklimatik-dresden.de/downloads/delphin6/Delphin6.updates");
	\endcode
	For the example above to work, the update-info file is expected to have a certain format:
	\code
	linux:Delphin_6.0.6_linux_2017-01-06.7z
	win:Delphin_6.0.5_win_int.exe
	win64:Delphin_6.0.5_win64_int.exe
	mac:Delphin_6.0.6_osx10.7.4_macosx_int.dmg
	---HeaderEnd---
	...changlog text follows...
	\endcode
	The file paths in above update info file are expexted to be relative to the path were D6.updates resides in
	or full-fledged URLs. For the installer files, the prefix (before the first underscore) is expected to be followed
	by a full version number to be compared against. The keys in the key-value list are platform indicators.

	Example for use-case 3: try to retrieve update information return most recent version number.
	\code
	QtExt::AutoUpdater updater;
	connect(&updater, SIGNAL(updateInfoAvailable()), this, SLOT(onUpdateInfoAvailable()));
	updater.checkForUpdateInfo("http://www.bauklimatik-dresden.de/downloads/delphin6/Delphin6.updates");
	// within the slot 'onUpdateInfoAvailable()'

	switch (updater.m_updateInfoResult) {
		case AutoUpdater::UpdateInfoSuccess :
			std::cout << "New version = " << updater.m_newVersionNumber.toStdString() << std::endl;
			break;
		case AutoUpdater::ConnectionFailure : ;
		case AutoUpdater::NoNewVersion :
			std::cout << "No new version available" << std::endl;
			break; // can't update
	}
	\endcode
*/
class AutoUpdater : public QObject {
	Q_OBJECT
public:
	explicit AutoUpdater(QObject *parent = 0);

	/*! Call this function from main.cpp to check if a downloaded update installer is available on the specified
		location and run the installation if possible.
		If such an update file exists, it is checked if it was installed already (file + .run) exists. In this
		case the downloaded file and .run info file is removed. Otherwise, the update process is
		started and the .run file is created.
		\return Returns true, if a process with the update file was started (in this case the calling program should
			terminate right away).
	*/
	static bool installUpdateWhenAvailable(const QString & localPath);


	/*! Attempts to download the update-info file from a given URL and returns true if
		the download was successful.

		The member variables then contain the info needed to integrate the update information
		into the application.
		\code
		// look for updates file
		autoUpdater->checkForUpdateInfo(QUrl("http://www.bauklimatik-dresden.de/downloads/delphin6/Delphin6.updates"), "6.0.7");
		\endcode

		\param url The download url for the update-info file.
		\param LONG_VERSION Three digit version number, used to check if there is a new version available.
		\param interactive If true, a dialog is shown after the update info has been retrieved (or an error message dialog if
			it failed), that shows new version and change log (downloaded separately). If false, function emits the signal
			updateInfoRetrieved(). The receiver of the signal can then check and compare the version number and either
			start the update dialog manually, or show any other means of update notification.
		\param downloadFilePath Path, where downloaded update install shall be stored in.
		\param newestRejectedVersion Only applicable when "interactive=false", i.e. when
	*/
	void checkForUpdateInfo(const QString & url, const char * const LONG_VERSION, bool interactive, QString downloadFilePath,
							QString newestRejectedVersion);

	/*! Shows a message box saying that there is no newer version available and shows the changelog file that
		was downloaded from the webpage.
	*/
	void showChangelogInfo(QWidget * w);

	/*! Shows a dialog with update info, using the content of the member variables m_newVersion and m_changeLogText for display
		in the dialog.
		Updates m_newestRejectedVersion when update was skipped.
	*/
	static void showUpdateDialog(QWidget * parent, QString currentVersion, QString newVersion,
								 QString changeLogText, QString downloadUrl, QString downloadFilePath, bool & rejected);

signals:
	/*! Signals is emitted when checkForUpdateInfo() was called in non-interactive mode.
		\param res Result of html request: 0 - ok, 1 - no update info (or no update file for this platform), 2 - connection error.
		\param newVersion New version available for current platform.
	*/
	void updateInfoRetrieved(int res, QString newVersion);

	/*! Emitted, when update dialog was cancelled by user through "skip-update" button. */
	void updateRejected(QString rejectedVersion);

private slots:
	/*! Triggered by network reply when download of update info has finished. */
	void downloadFinished(QNetworkReply *reply);

private:
	/*! If true, a download attempt for update info is followed by either an error message box or a update-download dialog. */
	bool					m_interactive;
	/*! Change log text (should be plain text). */
	QString					m_changeLogText;
	/*! New version available on server for current platform. */
	QString					m_newVersion;
	/*! Current version to be compared against version reported from server. */
	QString					m_currentVersion;
	/*! Path to the download info directory. */
	QString					m_updateInfoDownloadPath;

	/*! Url to update-installer. */
	QString					m_downloadUrl;

	/*! Local file path to downloaded update installer. */
	QString					m_downloadFilePath;

	/*! Holds the version string of the programm version that was last rejected for update.
		For example, when the current program version is 2.0.15, and the version 2.0.16 is available, and the user has
		clicked on "Skip update", m_newestRejectedVersion will be set to 2.0.16.
		In the next update check, the program version and m_newestRejectedVersion will be compared and the newer one
		will be used in the check for "current version". Thus, in the next call to the software, 2.0.16 will be compared
		to the available 2.0.16 version and the update info won't be shown.
	*/
	QString					m_newestRejectedVersion;

	/*! Network manager, used to execute network requests. */
	QNetworkAccessManager	*m_networkManager;
};

} // namespace QtExt

#endif // QtExt_AutoUpdaterH
