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
class QTableWidgetItem;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void loadData();

	void update(int tableWidth);

	void updateTable(const IBK::Time &filterDate);

protected:
	void resizeEvent(QResizeEvent *event) override;

	void showEvent(QShowEvent *event) override;

private slots:
	void readData();

	void setTableHeader();

	void on_tableWidget_itemChanged(QTableWidgetItem *item);

	void on_pushButton_clicked();

	void on_pushButtonMap_clicked();

	void on_pushButtonUpdate_clicked();

private:

	Ui::MainWindow						*m_ui;

	/*! Description input of all stations.
		key of map is station id
	*/
	std::map<unsigned int, DWDDescriptonData> m_descDataMap;

	QStandardItemModel					*m_model;
	QProgressDialog						*m_progressDlg;
	QTimer								m_progressTimer;

};

#endif // MAINWINDOW_H
