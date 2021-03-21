#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <IBK_NotificationHandler.h>
#include <IBK_messages.h>
#include <IBK_FileReader.h>
#include <QtExt_Directories.h>
#include <QtExt_ValidatingLineEdit.h>

#include <QCheckBox>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QPicture>
#include <QProgressDialog>
#include <QTableWidgetItem>
#include <QResizeEvent>

#include "DWDData.h"
#include "DWDDownloader.h"

#include "Constants.h"
//#include "DWD_CheckBox.h"

class ProgressNotify : public IBK::NotificationHandler{
public:
	ProgressNotify( QProgressDialog *dlg):
		m_dlg(dlg)
	{}

	/*! Reimplement this function in derived child 'notification' objects
		to provide whatever notification operation you want to perform.
	*/
	virtual void notify() {
		FUNCID(notify);
		if(m_dlg->value() == m_value)
			return;
		m_dlg->setValue(m_value);
		qApp->processEvents();
		if(m_dlg->wasCanceled()){
			throw IBK::Exception("Canceled", FUNC_ID);
		}
	}

	/*! Reimplement this function in derived child 'notification' objects
		to provide whatever notification operation you want to perform.

		The default implementation calls notify().
	*/
	virtual void notify(double p) {
		m_value = 100 * p;
		notify();
	}

	QProgressDialog			*m_dlg;
	int						m_value;
};

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_ui(new Ui::MainWindow),
	m_progressDlg(nullptr)
{
	m_ui->setupUi(this);

	m_model = new QStandardItemModel();

	m_ui->tableWidget->setColumnCount(9);
	m_ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Station Id"));
	m_ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Location Longitude Deg"));
	m_ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Location Latitude Deg"));
	m_ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Name"));
	m_ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Country"));
	m_ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("T_air + rH"));
	m_ui->tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Radiation"));
	m_ui->tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Wind"));
	m_ui->tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Pressure"));
	m_ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
	m_ui->tableWidget->horizontalHeader()->setMinimumHeight(40);
	m_ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	//m_ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//m_ui->tableWidget->setst

	testFunc();

	resize(1500,400);
	update(1400);
}


MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::testFunc(){

	//download all files
	DWDDescriptonData  descData;


	QStringList sl = descData.downloadDescriptionFiles();
//	DWDDownloader downloader(this);
//	downloader.append(sl);

	QMessageBox::information(this, QString(), "Please wait update description data from website.");

	DWDDownloaderDirk dwdd;
	for(const QString &s : sl){
		dwdd.m_urlString = s;
		dwdd.startDownload();
		QTime dieTime= QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < dieTime)
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}

//	QMessageBox::warning(this, QString(), "vor return");

	readData();


//	DWDData data;
//	data.m_startTime = IBK::Time(1981,0,0,0);
//	data.m_intervalDuration = 3600;

//	filepath = IBK::Path("../../data/Tests/produkt_tu_stunde_20190912_20210314_00044.txt");
//	IBK::FileReader fileReader(filepath);

//	//check if file is valid
//	if(!fileReader.valid()){
//		QMessageBox::warning(this, QString(),QString("File '%1' is not valid").arg(QString::fromStdString(filepath.absolutePath().c_str())));
//		return;
//	}
//	std::vector<std::string> lines;
//	//fill lines vector
//	fileReader.readAll(filepath, lines,std::vector<std::string>{"\n"});

//	std::set<DWDData::DataType> dataSet;

//	dataSet.insert(DWDData::DT_AirTemperature);
//	dataSet.insert(DWDData::DT_RelativeHumidity);

//	for (unsigned int i=1; i<lines.size(); ++i) {
//		data.addDataLine(lines[i], dataSet);
	//	}
}

void MainWindow::update(int tableWidth) {
	QTableWidget &tw = *m_ui->tableWidget;

	// resize cols
	tw.setColumnWidth(0, tableWidth / 18);
	tw.setColumnWidth(1, tableWidth / 9);
	tw.setColumnWidth(2, tableWidth / 9);
	tw.setColumnWidth(3, tableWidth * 7 /18 );
	tw.setColumnWidth(4, tableWidth / 9);
	tw.setColumnWidth(5, tableWidth / 18);
	tw.setColumnWidth(6, tableWidth / 18);
	tw.setColumnWidth(7, tableWidth / 18);
	tw.setColumnWidth(8, tableWidth / 18);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	// resize event is triggered
	// get tablewidget width
	int width = m_ui->tableWidget->width() - 80;

	update(width);
}

void MainWindow::readData() {
	//read all decription files
	DWDDescriptonData descData;
	descData.readAllDescriptions(m_descDataMap);


	//fill table view with these data
	//auto model = m_ui->tableView->model();
	unsigned int counter =0;
	QTableWidget &tw =  *m_ui->tableWidget;
	tw.blockSignals(true);

	tw.setSelectionBehavior(QAbstractItemView::SelectRows);
	tw.setSelectionMode(QAbstractItemView::SingleSelection);

	for (std::map<unsigned int, DWDDescriptonData>::const_iterator	it = m_descDataMap.begin();
																	it != m_descDataMap.end();
																	++it){
		tw.insertRow(counter);
		tw.setItem(counter, 0, new QTableWidgetItem(QString::number(it->second.m_id)));

		tw.setItem(counter, 0, new QTableWidgetItem(QString::number(it->second.m_id)));
		tw.setItem(counter, 1, new QTableWidgetItem(QString::number(it->second.m_longitude)));
		tw.setItem(counter, 2, new QTableWidgetItem(QString::number(it->second.m_latitude)));
		tw.setItem(counter, 3, new QTableWidgetItem(QString::fromStdString(it->second.m_name)));
		tw.setItem(counter, 4, new QTableWidgetItem(QString::fromStdString(it->second.m_country)));

		for (unsigned int i=0;i<5 ; ++i)
			tw.item(counter,i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		for (unsigned int i=0;i<4; ++i) {
			bool checkable = false;
			switch (i) {
				case 0:{
					if(it->second.m_data[DWDDescriptonData::D_TemperatureAndHumidity] != 0)
						checkable = true;
				}break;
				case 1:{
					if(it->second.m_data[DWDDescriptonData::D_Solar] != 0)
						checkable = true;

				}break;
				case 2:{
					if(it->second.m_data[DWDDescriptonData::D_Wind] != 0)
						checkable = true;

				}break;
				case 3:{
					if(it->second.m_data[DWDDescriptonData::D_Pressure] != 0)
						checkable = true;

				}break;

			}

			QTableWidgetItem *item = new QTableWidgetItem();
			if(checkable){
				item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
				item->setCheckState(Qt::Unchecked);
			}
			else
				item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

			tw.setItem(counter,5+i,item);

		}

		++counter;
	}
	tw.blockSignals(false);
}



void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item) {
	m_ui->tableWidget->selectRow(item->row() );
	if(item->column() >4){
		// get checked stated, if item was checked, uncheck all others
		if (item->checkState() == Qt::Checked) {
			// prevent events to fire
			m_ui->tableWidget->blockSignals(true);
			for (int i=0; i<m_ui->tableWidget->rowCount(); ++i) {
				QTableWidgetItem *newItem = m_ui->tableWidget->item(i, item->column());
				bool isUserCheckable = newItem->flags().testFlag(Qt::ItemIsUserCheckable);

				if (i == item->row() || !isUserCheckable)
					continue;

				m_ui->tableWidget->item(i, item->column())->setCheckState(Qt::Unchecked);
			}
			m_ui->tableWidget->blockSignals(false);
		}
	}
}






/// TODO
/// Herunterladen der Beschreibungsdateien
/// Lesen der Stationsbeschreibungsdateien
///
/* Stations_id von_datum bis_datum Stationshoehe geoBreite geoLaenge Stationsname Bundesland
----------- --------- --------- ------------- --------- --------- ----------------------------------------- ----------
012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
00003 19500401 20110331            202     50.7827    6.0941 Aachen                                   Nordrhein-Westfalen
00044 20070401 20210314             44     52.9336    8.2370 Großenkneten                             Niedersachsen
*/
/// sind für die ausgewählte datei daten vorhanden
/// Intervalfestlegung
/// Temperatur Feuchte
/// Strahlung
/// Wind, etc...
///
/// Daten runterladen
/// Fehlerprüfung
///		Fehlwerte korrigieren
/// fertigstellen für CCM (epw) oder ccd (nicht jahresscheiben)


void MainWindow::on_pushButton_clicked(){
	std::vector<int> rows(4,-1);
	//find selected elements
	QTableWidget & tw = *m_ui->tableWidget;
	for (unsigned int row=0; row<tw.rowCount(); ++row) {
		for(unsigned int col=5; col<tw.columnCount(); ++col){

			if(tw.item(row,col)->checkState() == Qt::Checked){
				rows[col-5] = IBK::string2val<int>(tw.item(row,0)->text().toStdString());
				//QMessageBox::information(this, QString(), "col: " + QString::number(col) + " | station id: " + QString::number(rows[col-5]) );
			}
		}
	}

	std::vector<QString> filenames; //hold filenames for download
	std::vector<DWDData::DataType>	types{DWDData::DT_AirTemperature, DWDData::DT_RadiationDiffuse,
									DWDData::DT_WindDirection,DWDData::DT_Pressure};

	int delayTime = 3; //sec to wait after download

	//download the data (zip)
	for(unsigned int i=0; i<4; ++i){
		if(rows[i] != -1){
			DWDData dwdData;
			DWDDownloaderDirk dwdd;
			dwdd.m_urlString = dwdData.urlFilename(types[i], QString::number(rows[i]).rightJustified(5,'0'));
			QFileInfo fileInfo = QUrl(dwdd.m_urlString).path();
			filenames.push_back(fileInfo.fileName());
			//QMessageBox::information(this, QString(), dwdd.m_urlString);
			dwdd.startDownload();
			QTime dieTime= QTime::currentTime().addSecs(delayTime);
			while (QTime::currentTime() < dieTime)
				QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}
	}

	QMessageBox::information(this, QString(), "jetzt sollten die Daten da sein");

	//Check if all downloads are valid
	//create a vector with valid files
	std::vector<IBK::Path>	checkedFiles(4);

	for(unsigned int i=0; i<4; ++i){
		IBK::Path checkfile("../../data/Tests/" + filenames[i].toStdString());
		if(!checkfile.exists() && rows[i] != -1){
			QString cat;
			switch (types[i]) {
				case DWDData::DT_AirTemperature:	cat = "Temperature and relative Humidity";		break;
				case DWDData::DT_RadiationDiffuse:	cat = "Radiation"						;		break;
				case DWDData::DT_WindDirection:		cat = "Wind";									break;
				case DWDData::DT_Pressure:			cat = "Pressure";								break;
			}
			QMessageBox::warning(this, QString(), QString("Download of file '%1' was not successfull. Category: '%2'").arg(filenames[i]).arg(cat));
		}
		else if(rows[i] == -1)
			continue;
		else
			checkedFiles[i] = checkfile;
	}

	std::vector<IBK::Path>	productFiles(4);
	//open the zip
	//find file with name 'produkt_....'
	///TODO Dirk nur zu Probe muss später ersetzt werden
	productFiles[0] = IBK::Path("../../data/Tests/produkt_tu_stunde_20190917_20210319_00183.txt");

	//create extract folder

	//extract file from zip

	//create the file path names and according data types for reading
	std::map<IBK::Path, std::set<DWDData::DataType>> filenamesForReading;
	for(unsigned int i=0; i<4; ++i){
		if(productFiles[i] == IBK::Path())
			continue;
		switch (i) {
			case 0:
				filenamesForReading[productFiles[i]] = std::set<DWDData::DataType>{DWDData::DT_AirTemperature, DWDData::DT_RelativeHumidity}; break;
			case 1:
				filenamesForReading[productFiles[i]] = std::set<DWDData::DataType>{DWDData::DT_RadiationDiffuse,DWDData::DT_RadiationGlobal, DWDData::DT_RadiationLongWave}; break;
			case 2:
				filenamesForReading[productFiles[i]] = std::set<DWDData::DataType>{DWDData::DT_WindDirection,DWDData::DT_WindSpeed}; break;
			case 3:
				filenamesForReading[productFiles[i]] = std::set<DWDData::DataType>{DWDData::DT_Pressure}; break;
		}
	}
	//read data
	DWDData dwdData;
	dwdData.createData(filenamesForReading);

	//copy all data in range and create an epw
}
