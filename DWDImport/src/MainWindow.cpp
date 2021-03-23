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
#include <QTimer>
#include <QThread>
#include <QMessageBox>

#include <JlCompress.h>

#include "DWDData.h"
#include "DWDDownloader.h"
#include "DWDMap.h"

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

	m_ui->lineEditYear->setup(1950,2023,tr("Year of interest."), true, true);

	m_ui->tableWidget->verticalHeader()->setDefaultSectionSize(19);
	m_ui->tableWidget->verticalHeader()->setVisible(false);
	m_ui->tableWidget->horizontalHeader()->setMinimumSectionSize(19);
	m_ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ui->tableWidget->setAlternatingRowColors(true);
//	m_ui->tableWidget->setSortingEnabled(true);
//	m_ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
	QFont f;
	f.setPointSizeF(f.pointSizeF()*0.8);
	m_ui->tableWidget->setFont(f);
	m_ui->tableWidget->horizontalHeader()->setFont(f); // Note: on Linux/Mac this won't work until Qt 5.11.1 - this was a bug between Qt 4.8...5.11.1

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


	QStringList urls = descData.downloadDescriptionFiles();
//	DWDDownloader downloader(this);
//	downloader.append(sl);




//	connect(dwdd, &DWDDownloaderDirk::fini, )

	DWDDownloader manager(this);
	manager.m_urls = urls;
	connect( &manager, &DWDDownloader::finished, this, &MainWindow::readData );

	QTimer::singleShot(0, &manager, SLOT(execute() ) );

	while ( manager.m_isRunning ) // wait for data to be downloaded (the little bit dirty way)
		QMessageBox::information(this, "Downloading", "Download is running" );
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
	int width = m_ui->tableWidget->viewport()->width();

	update(width);
}

void MainWindow::showEvent(QShowEvent * event) {
	// update ui with table widget width
	update(m_ui->tableWidget->viewport()->width() );
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
		tw.setItem(counter, 3, new QTableWidgetItem(QString::fromLatin1(it->second.m_name.c_str())));
		tw.setItem(counter, 4, new QTableWidgetItem(QString::fromLatin1(it->second.m_country.c_str())));

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

	std::vector<QString> filenames(4); //hold filenames for download
	std::vector<DWDData::DataType>	types{DWDData::DT_AirTemperature, DWDData::DT_RadiationDiffuse,
									DWDData::DT_WindDirection,DWDData::DT_Pressure};

	DWDDownloader manager(this);
	connect( &manager, &DWDDownloader::finished, this, &MainWindow::readData );
	//download the data (zip)
	for(unsigned int i=0; i<4; ++i){
		if(rows[i] != -1){
			DWDData dwdData;
			manager.m_urls << dwdData.urlFilename(types[i], QString::number(rows[i]).rightJustified(5,'0'));
			filenames[i] = dwdData.filename(types[i], QString::number(rows[i]).rightJustified(5,'0'));
		}
	}
	if(!manager.m_urls.empty())
		QTimer::singleShot(0, &manager, SLOT(execute() ) );

	//Check if all downloads are valid
	//create a vector with valid files
	std::vector<IBK::Path>	checkedFiles(4);

	for(unsigned int i=0; i<4; ++i){
		if(rows[i] == -1)
			continue;
		IBK::Path checkfile("../../data/Tests/" + filenames[i].toStdString() + ".zip");
		if(!checkfile.exists()){
			QString cat;
			switch (types[i]) {
				case DWDData::DT_AirTemperature:	cat = "Temperature and relative Humidity";		break;
				case DWDData::DT_RadiationDiffuse:	cat = "Radiation"						;		break;
				case DWDData::DT_WindDirection:		cat = "Wind";									break;
				case DWDData::DT_Pressure:			cat = "Pressure";								break;
			}
			QMessageBox::warning(this, QString(), QString("Download of file '%1' was not successfull. Category: '%2'").arg(filenames[i]+".zip").arg(cat));
		}
		else
			checkedFiles[i] = checkfile;
	}

	//open the zip
	//find file with name 'produkt_....'
	//create the file path names and according data types for reading
	std::vector<IBK::Path>	checkedFileNames(4);
	std::map<IBK::Path, std::set<DWDData::DataType>> filenamesForReading;
	for (int i=0; i<filenames.size(); ++i) {
		if ( filenames[i].isEmpty() )
			continue;

		QStringList filesInArchive = JlCompress::getFileList(checkedFiles[i].str().c_str());
		QString searchedFile;
		QStringList filesExtracted;
		for ( QString fileName : filesInArchive ) {
			if ( "produkt" == fileName.mid(0,7) ){ // we find the right file
				// extract file
				searchedFile = fileName;
				filesExtracted << JlCompress::extractFile( checkedFiles[i].str().c_str(), fileName, "../../data/Tests/extractedFiles/" + searchedFile);
				checkedFileNames[i] = IBK::Path("../../data/Tests/extractedFiles/" + searchedFile.toStdString() );
			}
		}

		if ( filesExtracted.empty() )
			QMessageBox::warning(this, QString(), QString("File %1 could not be extracted").arg(searchedFile));
	}

	//create extract folder

	//extract file from zip

	for(unsigned int i=0; i<4; ++i){
		if(checkedFileNames[i] == IBK::Path())
			continue; // skip empty states
		switch (i) {
			case 0:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_AirTemperature, DWDData::DT_RelativeHumidity}; break;
			case 1:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_RadiationDiffuse,DWDData::DT_RadiationGlobal, DWDData::DT_RadiationLongWave}; break;
			case 2:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_WindDirection,DWDData::DT_WindSpeed}; break;
			case 3:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_Pressure}; break;
		}
	}
	//read data
	DWDData dwdData;
	dwdData.m_startTime = IBK::Time(m_ui->lineEditYear->text().toInt(),0);
	dwdData.createData(filenamesForReading);

//	dwdData.writeTSV(2001);

	//copy all data in range and create an epw
	dwdData.exportEPW(2019);
}

void MainWindow::on_pushButtonMap_clicked() {
	double latitude = m_ui->lineEditLatitude->text().toDouble();
	double longitude = m_ui->lineEditLongitude->text().toDouble();

	DWDMap::getLocation(latitude, longitude, this);

	m_ui->lineEditLatitude->setText(QString::number(latitude) );
	m_ui->lineEditLongitude->setText(QString::number(longitude) );
}
