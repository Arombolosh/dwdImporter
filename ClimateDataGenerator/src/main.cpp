#include <IBK_messages.h>
#include <IBK_Exception.h>

#include <QApplication>
#include <QDir>
#include <QProxyStyle>
#include <QStyleOptionViewItem>

#include <QtExt_Directories.h>

#include "DWDMainWindow.h"
#include "DWDMessageHandler.h"

#include "Constants.h"

/*! qDebug() message handler function, redirects debug messages to IBK::IBK_Message(). */
void qDebugMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	(void) type;
	std::string contextstr;
	if (context.file != nullptr && context.function != nullptr)
		contextstr = "[" + std::string(context.file) + "::" + std::string(context.function) + "]";
	IBK::IBK_Message(msg.toStdString(), IBK::MSG_DEBUG, contextstr.c_str(), IBK::VL_ALL);
}

int main(int argc, char* argv[]) {
	FUNCID(main);
	QApplication a( argc, argv );

	// install message handler to catch qDebug()
	qInstallMessageHandler(qDebugMsgHandler);

	// *** Create log file directory and setup message handler ***
	QDir baseDir;
	QtExt::Directories::appname = PROGRAM_NAME;
	baseDir.mkpath(QtExt::Directories::userDataDir());

	DWDMessageHandler messageHandler;
	IBK::MessageHandlerRegistry::instance().setMessageHandler( &messageHandler );

	std::string errmsg;
	messageHandler.openLogFile(QtExt::Directories::globalLogFile().toUtf8().data(), false, errmsg);

	// Set stylsheet
	QFile darkStyle(":/style/style.qss");
	darkStyle.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(darkStyle.readAll());
	qApp->setStyleSheet(styleSheet);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


	// *** Setup and show MainWindow and start event loop ***
	int res;
	try { // open scope to control lifetime of main window, ensure that main window instance dies before settings or project handler

		MainWindow w;
		// Dark style


		// add user settings related window resize at program start
#if defined(Q_OS_WIN)
		//w.showMaximized();
		w.showMaximized();
#elif defined(Q_OS_LINUX)
		w.show();
#else
		w.show();
#endif
		// we set the data to the tableWidget
		w.loadData();

		// start event loop
		res = a.exec();
	} // here our mainwindow dies, main window goes out of scope and UI goes down -> destructor does ui and thread cleanup
	catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();
		return EXIT_FAILURE;
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	// return exit code to environment

	return res;
	return 0;
}


