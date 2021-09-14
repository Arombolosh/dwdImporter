#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QMainWindow>
#include <QWidget>
#include <QProcess>
#include <QElapsedTimer>
#include <QStandardItemModel>

#include <IBK_Time.h>
#include <IBK_Path.h>

#include "DWDDescriptonData.h"
#include "DWDData.h"
#include "DWDTableModel.h"


namespace Ui {
	class MainWindow;
}

class QProgressDialog;
class QTableWidgetItem;
class DWDDownloader;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void loadData();


	void addToList(const QUrlInfo qUrlI);

	/*! Function that calculates all distances to the Reference location */
	void calculateDistances();


private slots:
	void readData();


	void on_pushButtonDownload_clicked();

	void on_pushButtonMap_clicked();

	void setProgress(int min, int max, int val);

	void on_radioButtonHistorical_toggled(bool checked);

	void on_lineEditYear_editingFinished();

private:

	Ui::MainWindow						*m_ui;

	DWDDownloader						*m_manager = nullptr;

	/*! Description input of all stations.
		key of map is station id
	*/
	std::map<unsigned int, DWDDescriptonData>	m_descDataMap;
	DWDData										m_dwdData;

	QStringList									m_filelist;

	QStandardItemModel							*m_model;
	QProgressDialog								*m_progressDlg;
	QElapsedTimer								m_progressTimer;

	/*! Table model instance for dwd data. */
	DWDTableModel								*m_dwdTableModel = nullptr;

};

#endif // MAINWINDOW_H
