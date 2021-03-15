#include <IBK_messages.h>
#include <IBK_Exception.h>

#include <QApplication>
#include <QDir>

#include <QtExt_Directories.h>

#include "MainWindow.h"

#include "Constants.h"

int main(int argc, char* argv[]) {
	FUNCID(main);
	QApplication a( argc, argv );
	// *** Create log file directory and setup message handler ***

	QDir baseDir;
	QtExt::Directories::appname = PROGRAM_NAME;
	baseDir.mkpath(QtExt::Directories::userDataDir());

	IBK::MessageHandler messageHandler;
	IBK::MessageHandlerRegistry::instance().setMessageHandler( &messageHandler );

	std::string errmsg;
	messageHandler.openLogFile(QtExt::Directories::globalLogFile().toUtf8().data(), false, errmsg);


	// *** Setup and show MainWindow and start event loop ***
	int res;
	try { // open scope to control lifetime of main window, ensure that main window instance dies before settings or project handler

		MainWindow w;

		// add user settings related window resize at program start
#if defined(Q_OS_WIN)
		//w.showMaximized();
		w.show();
#elif defined(Q_OS_LINUX)
		w.show();
#else
		w.show();
#endif
		// start event loop
		res = a.exec();
	} // here our mainwindow dies, main window goes out of scope and UI goes down -> destructor does ui and thread cleanup
	catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();
		return EXIT_FAILURE;
	}

	// return exit code to environment

	return res;
}


