#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QMainWindow>
#include <QWidget>
#include <QProcess>
#include <QElapsedTimer>
#include <QStandardItemModel>
#include <QFile>

#include <IBK_Time.h>
#include <IBK_Path.h>

#include <DM_MapDialog.h>

#include "DWDDescriptonData.h"
#include "DWDData.h"
#include "DWDTableModel.h"

#include <qwt_plot.h>

namespace Ui {
class MainWindow;
}

class QProgressDialog;
class QAbstractProxyModel;
class QTableWidgetItem;
class DWDDownloader;
class DWDMap;
class DWDSortFilterProxyModel;
class DWDProgressBar;
class DWDLogWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	/*! Download data from DWD server. */
	void loadDataFromDWDServer();

	/*! Sets all GUI states. */
	void setGUIState(bool guiState);

	/*! Downloads all dwd data. */
	void downloadData(bool showPreview = true, bool exportEPW = false);

	void addToList(const QUrlInfo qUrlI);

	/*! Function that calculates all distances to the Reference location. */
	void calculateDistances();

	/*! Inits all plots. */
	void initPlots();

	/*! Formats a qwt plot. */
	void formatQwtPlot(QwtPlot &plot, QDate startDate, QDate endDate, QString title, QString leftYAxisTitle, double yLeftMin, double yLeftMax, double yLeftStepSize,
					   bool hasRightAxis = false, QString rightYAxisTitle = "", double yRightMin = 0, double yRightMax = 100, double yRightStepSize = 0);


private slots:
	void convertDwdData();


	void on_pushButtonDownload_clicked();

	void on_pushButtonMap_clicked();

	void setProgress(int min, int max, int val);

	void on_radioButtonHistorical_toggled(bool checked);

	void on_lineEditNameFilter_textChanged(const QString &filter);

	void on_horizontalSliderDistance_valueChanged(int value);

	void on_pushButtonPreview_clicked();

	void on_toolButtonOpenDirectory_clicked();


private:

	Ui::MainWindow								*m_ui;

	/*! Pointer to download manager. */
	DWDDownloader								*m_manager = nullptr;

	/*! Pointer to Map Widget. */
	DM::MapDialog								*m_mapDialog = nullptr;

	/*! Description input of all existing DWD stations.
		\param key: station id
	*/
	std::vector<DWDDescriptonData>				m_descData;
	DWDData										m_dwdData;

	QStringList									m_filelist;

	DWDMap										*m_dwdMap;

	QStandardItemModel							*m_model;
	QProgressDialog								*m_progressDlg;

	/*! Table model instance for dwd data. */
	DWDTableModel								*m_dwdTableModel = nullptr;
	DWDSortFilterProxyModel						*m_proxyModel = nullptr;
	QAbstractProxyModel							*m_abstractProxyModel = nullptr;

	QString										m_fileName;

	DWDLogWidget                               *m_logWidget = nullptr;

};

#endif // MAINWINDOW_H
