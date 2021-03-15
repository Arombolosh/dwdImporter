#include "QtExt_Directories.h"

#include <QCoreApplication>
#if QT_VERSION < 0x050000
	#include <QDesktopServices>
#else
	#include <QStandardPaths>
#endif
#include <QDir>
#include <QSettings>

#include <IBK_configuration.h>
#include <IBK_messages.h>

namespace QtExt {

// You need to set values to these variables early on in your main.cpp
QString Directories::appname; // for example "Therakles"
QString Directories::devdir; // for example "TheraklesApp"

/*! Utility function for conversion of a QString to a trimmed std::string in utf8 encoding. */
inline std::string QString2trimmedUtf8(const QString & str) {
	return str.trimmed().toUtf8().data();
}


QString Directories::resourcesRootDir() {
	QString installPath = qApp->applicationDirPath();

#ifdef IBK_DEPLOYMENT
	// deployment mode

#if defined(Q_OS_WIN)
	// in Deployment mode, resources are below install directory
	return installPath + "/resources";
#elif defined(Q_OS_MAC)
	// in deployment mode, we have them in <appname>.app/Contents/resources
	return installPath + "/../resources";
#elif defined(Q_OS_UNIX)

	// in deployment mode, we have them, for example, in "/usr/share/<appname>" or "/usr/local/share/<appname>"
	// when app is installed system-wide

	// otherwise, for local install, we have them inside the app archive directory structure

	QString resRootPath;
	if (installPath.indexOf("/usr/bin") == 0)
		resRootPath = "/usr/share/" + appname;
	else if (installPath.indexOf("/usr/local/bin") == 0)
		resRootPath = "/usr/local/share/" + appname;
	else
		resRootPath = installPath + "/../resources";
	return resRootPath;
#endif


#else // IBK_DEPLOYMENT

	// development (IDE) mode

	// resources are expected in devname/resources directory
#if defined(Q_OS_MAC)
	return QFileInfo(installPath + "/../../../../../" + devdir + "/resources").absoluteFilePath();
#else
	return QFileInfo(installPath + "/../../" + devdir + "/resources").absoluteFilePath();
#endif

#endif // IBK_DEPLOYMENT
}


QString Directories::databasesDir() {
#ifdef IBK_DEPLOYMENT

	// deployment mode, databases are in the same directory as resources
	return resourcesRootDir();

#else // IBK_DEPLOYMENT

	// during development, databases are expected in the data directory
	QString installPath = qApp->applicationDirPath();

#if defined(Q_OS_WIN)
	// in development mode, we have the resources in the data directory
	// executables are build in bin/debug or bin/release
	return installPath + "/../../data";
#elif defined(Q_OS_MAC)
	// in development mode, we have the resources outside the bundle
	return installPath + "/../../../../../data";
#elif defined(Q_OS_UNIX)
	return installPath + "/../../data";
#else
#error Implement this for your platform/compiler
#endif

#endif // IBK_DEPLOYMENT
}


QString Directories::translationsDir() {
	return resourcesRootDir() + "/translations";
}


QString Directories::userDataDir() {
	// we have different user data directories, based on OS
#if defined(Q_OS_WIN)
	// on Windows, we store user data unter %HOME%/AppData/Roaming
	QString fname = QDir::toNativeSeparators(QDir::home().absolutePath() + "/AppData/Roaming/" + appname);
#else
	// on Unix/Mac OS we store user data under home directory
	QString fname = QDir::toNativeSeparators(QDir::home().absolutePath() + "/.local/share/" + appname);
#endif // Q_OS_WIN
	QFileInfo fpath(fname);
	return fpath.absoluteFilePath();
}


QString Directories::tmpDir() {
#if defined(Q_OS_WIN)
	// on Windows, we store user data unter %TMP%
#if QT_VERSION < 0x050000
	return QDesktopServices::storageLocation(QDesktopServices::TempLocation);
#else // QT_VERSION < 0x050000
	return QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#endif // QT_VERSION < 0x050000
#else
	// on Unix/Mac OS we use global tmp dir
	return "/tmp";
#endif // Q_OS_WIN
}


QString Directories::globalLogFile() {
	return userDataDir() + "/"+appname+".log";
}


QString Directories::generateTempFileName() {

	const char * const FUNC_ID = "[Directories::generateTempFileName]";

	QString tmpDirectory = tmpDir();

	unsigned int i = 0;
	for (;;) {
		++i;
		// generate file path
		QString fpath = tmpDirectory + QString("/ibk_tmp%1").arg(i);
		// check if file exists
		if (QFileInfo(fpath).exists())
			continue; // next index
		// check if file can be written into
		if (!checkForWriteAccess(fpath)) {
			IBK::IBK_Message( IBK::FormatString("Cannot write to temporary file '%1'").arg( QString2trimmedUtf8(fpath) ),
							  IBK::MSG_WARNING, FUNC_ID);
			break; // no write access to tmp
		}
		return fpath; // file name found
	}

	// cannot write in global tmp, use tmp subdir in user data dir
	tmpDirectory = userDataDir() + "/tmp";
	if (!QDir(tmpDirectory).exists()) {
		if (!QDir().mkdir(tmpDirectory)) {
			IBK::IBK_Message( IBK::FormatString("Cannot create user-tmp directory '%1'").arg( QString2trimmedUtf8(tmpDirectory) ),
							  IBK::MSG_ERROR, FUNC_ID);
			return QString();
		}
	}

	for (;;) {
		++i;
		// generate file path
		QString fpath = tmpDirectory + QString("/ibk_tmp%1").arg(i);
		// check if file exists
		if (QFileInfo(fpath).exists())
			continue; // next index
		// check if file can be written into
		if (!checkForWriteAccess(fpath)) {
			IBK::IBK_Message( IBK::FormatString("Cannot write to temporary file '%1'").arg( QString2trimmedUtf8(fpath) ),
							  IBK::MSG_ERROR, FUNC_ID);
			return QString();
		}
		return fpath; // file name found
	}
}


bool Directories::checkForWriteAccess(const QString & newFileName) {
	if (QFile(newFileName).exists())
		return false;
	// check if file can be written into
	QFile f(newFileName);
	if (f.open(QIODevice::WriteOnly)) {
		f.remove();
		return true;
	}
	return false;
}


QString Directories::updateFilePath() {
	// In user-data-dir 'updates' check for
#ifdef _WIN32
	QDir userDir(Directories::userDataDir() + "/updates");

#ifdef _WIN64
	QString updateFile = userDir.absoluteFilePath("update_win64.exe");
#else
	QString updateFile = userDir.absoluteFilePath("update.exe");
#endif
	return updateFile;
#elif defined(Q_OS_MAC)
	// "version" will be replaced by new version number
	return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+"/" + appname + "_version.dmg";
#else
	// "version" will be replaced by new version number
	return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+"/" + appname + "_version.7z";
#endif
}

// helper function to remove a complete directory hierarchie
bool Directories::removeDirRecursively(const QString & directory) {
	QDir dir(directory);

	if (!dir.exists(directory))
		return false;

	// entryInfoList() filter options taken from John's Blog, http://http://john.nachtimwald.com, June 8th, 2010
	QFileInfoList fInfoList = dir.entryInfoList(
		QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs |
		QDir::Files, QDir::DirsFirst);
	foreach(QFileInfo fInfo, fInfoList) {
		if (fInfo.isDir()) {
			if (!removeDirRecursively(fInfo.absoluteFilePath()))
				return false;
		}
		else {
			if (!QFile::remove(fInfo.absoluteFilePath()))
				return false;
		}
	}
	if (!dir.rmdir(directory))
		return false;

	return true;
}

} // namespace QtExt
