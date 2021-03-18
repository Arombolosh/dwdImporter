#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QMainWindow>
#include <QWidget>
#include <QProcess>
#include <QTimer>
#include <QStandardItemModel>

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

	void testFunc();

private slots:
	void on_checkboxChecked(int state, int row, int col);

	void on_checkBox_toggled(bool checked);

private:

	/*! Reads a decription file and adds all stations in stationDescription map. The dataType must be given. */
	void readDescription(const IBK::Path &filepath, std::map<unsigned int, DWDDescriptonData> &stationDescription, const DWDDescriptonData::Data &dataType);

	Ui::MainWindow						*m_ui;

	QStandardItemModel					*m_model;
	QProgressDialog						*m_progressDlg;
	QTimer								m_progressTimer;

};

#endif // MAINWINDOW_H
