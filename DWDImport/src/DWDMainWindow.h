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
class QAbstractProxyModel;
class QTableWidgetItem;
class DWDDownloader;
class DWDMap;
class DWDSortFilterProxyModel;

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

	void on_lineEditDistance_textChanged(const QString &arg1);

private:

	Ui::MainWindow						*m_ui;

	DWDDownloader						*m_manager = nullptr;

	/*! Description input of all stations.
		key of map is station id
	*/
	std::vector<DWDDescriptonData>				m_descData;
	DWDData										m_dwdData;

	QStringList									m_filelist;

	DWDMap										*m_dwdMap;

	QStandardItemModel							*m_model;
	QProgressDialog								*m_progressDlg;
	QElapsedTimer								m_progressTimer;

	/*! Table model instance for dwd data. */
	DWDTableModel								*m_dwdTableModel = nullptr;
	DWDSortFilterProxyModel						*m_proxyModel = nullptr;
	QAbstractProxyModel							*m_abstractProxyModel = nullptr;

};

#endif // MAINWINDOW_H
