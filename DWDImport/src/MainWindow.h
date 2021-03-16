#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QMainWindow>
#include <QWidget>
#include <QProcess>
#include <QTimer>

#include <IBK_Time.h>
#include <IBK_Path.h>

#include "DWDDescriptonData.h"


namespace Ui {
class MainWindow;
}
class QProgressDialog;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:



	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:


private:

	/*! Reads a decription file and adds all stations in stationDescription map. The dataType must be given. */
	void readDescription(const IBK::Path &filepath, std::map<unsigned int, DWDDescriptonData> &stationDescription, const DWDDescriptonData::Data &dataType);

	Ui::MainWindow						*m_ui;

	QProgressDialog						*m_progressDlg;
	QTimer								m_progressTimer;

};

#endif // MAINWINDOW_H
