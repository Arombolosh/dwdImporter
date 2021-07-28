#include "DWDMainWindow.h"
#include "ui_DWDMainWindow.h"

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
#include <QItemDelegate>
#include <QtNumeric>

#include <qftp.h>

#include <JlCompress.h>

#include "DWDDownloader.h"
#include "DWDMap.h"
#include "DWDDelegate.h"
#include "DWDData.h"

#include "Constants.h"
//#include "DWD_CheckBox.h"

class ProgressNotify : public IBK::NotificationHandler{
public:
	ProgressNotify( QProgressBar *bar):
		m_bar(bar)
	{}

	/*! Reimplement this function in derived child 'notification' objects
		to provide whatever notification operation you want to perform.
	*/
	virtual void notify() {
		FUNCID(notify);
		if(m_bar->value() == m_value)
			return;
		m_bar->setValue(m_value);
		qApp->processEvents();
//		if(m_bar->wasCanceled()){
//			throw IBK::Exception("Canceled", FUNC_ID);
//		}
	}

	/*! Reimplement this function in derived child 'notification' objects
		to provide whatever notification operation you want to perform.

		The default implementation calls notify().
	*/
	virtual void notify(double p) {
		m_value = 100 * p;
		notify();
	}

	QProgressBar			*m_bar;
	int						m_value;
};

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_ui(new Ui::MainWindow),
	m_progressDlg(nullptr)
{
	m_ui->setupUi(this);

	m_model = new QStandardItemModel();
	m_ui->radioButtonRecent->blockSignals(true);
	m_ui->radioButtonRecent->setChecked(true);
	m_ui->radioButtonRecent->blockSignals(false);


	m_ui->lineEditLatitude->setup(-90,90, "Latitude in Deg", true, true);
	m_ui->lineEditLongitude->setup(-180,180, "Longitude in Deg", true, true);
	m_ui->lineEditLatitude->setText("51.03");
	m_ui->lineEditLongitude->setText("13.7");

	m_ui->lineEditYear->setup(1950,2023,tr("Year of interest."), true, true);
	m_ui->lineEditYear->setText("2020");

	setTableHeader();

	m_progressTimer.start();

	connect( &m_dwdData, &DWDData::progress, this, &MainWindow::setProgress );

	resize(1500,400);
	update(1300);
}

/// TODO Stephan
/// bitte noch die Spalten sortierbar gestalten
/// und die checkboxen in allen spalten zurechtrücken
/// und die clicks auf die checkboxen setzen
/// distance spalte einführen und berechnen
/// danke

MainWindow::~MainWindow() {
	delete m_ui;
}

void MainWindow::loadData(){

	//download all files
	DWDDescriptonData  descData;

	// get download links for data
	QStringList urls = descData.downloadDescriptionFiles(m_ui->radioButtonRecent->isChecked());

	// initiate download manager
	DWDDownloader manager(this);
	manager.m_urls = urls;
	manager.m_progress = m_ui->progressBar; // bisschen quatsch
	manager.m_label = m_ui->labelDownload; // bisschen quatsch
	connect( &manager, &DWDDownloader::finished, this, &MainWindow::readData );

	// start download in singleshot
	QTimer::singleShot(0, &manager, SLOT(execute() ) );

	// dirty way to wait till asynchronous download is finished
	while ( manager.m_isRunning ) // wait for data to be downloaded (the little bit dirty way)
		qApp->processEvents();
}

void MainWindow::update(int tableWidth) {

	// resize tablewidget
	QTableWidget &tw = *m_ui->tableWidget;

	// resize cols
	tw.setColumnWidth(0, tableWidth / 18);
	tw.setColumnWidth(1, tableWidth / 9);
	tw.setColumnWidth(2, tableWidth / 9);
	tw.setColumnWidth(3, tableWidth * 6 /18 );
	tw.setColumnWidth(4, tableWidth / 9);
	tw.setColumnWidth(5, tableWidth / 18);
	tw.setColumnWidth(6, tableWidth / 18);
	tw.setColumnWidth(7, tableWidth / 18);
	tw.setColumnWidth(8, tableWidth / 18);
	tw.setColumnWidth(9, tableWidth / 18);
}

void MainWindow::updateTable() {

	m_ui->tableWidget->blockSignals(true);
	IBK::Time filterDate (m_ui->lineEditYear->text().toInt(), 0);
	// take year and check for each data type (rad, wind, temp,..) if data is available
	// sort data
	unsigned int counter =0;
	QTableWidget &tw =  *m_ui->tableWidget;

	// reset table
	tw.clearContents();
	tw.setRowCount(0);

	for (std::map<unsigned int, DWDDescriptonData>::const_iterator	it = m_descDataMap.begin();
																	it != m_descDataMap.end();
																	++it){

		bool isItemVisible = false;

		for (unsigned int i=0;i<4; ++i) {
			bool checkable = false;
			/*! Start date. */
			IBK::Time startDate = it->second.m_startDate[i];

			/*! End date. */
			IBK::Time endDate = it->second.m_endDate[i];

			IBK::Time filterDateNextYear ( filterDate.year()+1, 0 );

			double secUntilStart = startDate.secondsUntil(filterDate);
			double secUntilEnd = filterDateNextYear.secondsUntil(endDate);

			if ( secUntilStart<0 || secUntilEnd<0 )
				continue;

			if (!isItemVisible) {
				tw.insertRow(counter);
				isItemVisible = true;
			}

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
			DWDDelegate *delegate = new DWDDelegate(this);
//			m_ui->tableWidget->setItemDelegateForColumn(5, delegate);

			if(checkable){
				item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
				item->setCheckState(Qt::Unchecked);
				item->setTextAlignment(Qt::AlignHCenter); // change the alignment
			}
			else
				item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

			tw.setItem(counter,6+i,item);
		}

		if ( !isItemVisible )
			continue;

		tw.setItem(counter, 0, new QTableWidgetItem(QString::number(it->second.m_id)));

		double lat1 = m_ui->lineEditLatitude->text().toDouble();
		double lon1 = m_ui->lineEditLongitude->text().toDouble();

		double lat2 = it->second.m_latitude;
		double lon2 = it->second.m_longitude;

		double lat = (lat1 + lat2) / 2 * 0.01745;
		double dx = 111.3 * cos(lat) * (lon1 - lon2);
		double dy = 111.3 * (lat1 - lat2);

		// calc distance
		double distance = std::sqrt( std::pow( dx, 2 ) + std::pow( dy, 2 ) );

		QTableWidgetItem *newItem = new QTableWidgetItem();
		newItem->setData(Qt::DisplayRole, distance);

		tw.setItem(counter, 1, newItem );
		tw.setItem(counter, 2, new QTableWidgetItem(QString::number(it->second.m_longitude)));
		tw.setItem(counter, 3, new QTableWidgetItem(QString::number(it->second.m_latitude)));
		tw.setItem(counter, 4, new QTableWidgetItem(QString::fromLatin1(it->second.m_name.c_str())));
		tw.setItem(counter, 5, new QTableWidgetItem(QString::fromLatin1(it->second.m_country.c_str())));

		for (unsigned int i=0;i<6 ; ++i)
			tw.item(counter,i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		++counter;
	}

//	tw.setSortingEnabled(true);
	tw.sortByColumn(1, Qt::AscendingOrder );

	m_ui->tableWidget->blockSignals(false);
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
	// read all decription files
	DWDDescriptonData descData;
	m_descDataMap.clear();
	descData.readAllDescriptions(m_descDataMap);

	// fill table view with these data
	// auto model = m_ui->tableView->model();
	unsigned int counter =0;
	QTableWidget &tw =  *m_ui->tableWidget;
	tw.blockSignals(true);

	tw.setSelectionBehavior(QAbstractItemView::SelectRows);
	tw.setSelectionMode(QAbstractItemView::SingleSelection);

	updateTable();
	tw.blockSignals(false);
}

void MainWindow::setTableHeader() {
	m_ui->tableWidget->setColumnCount(10);
	m_ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Station Id"));
	m_ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Distance"));
	m_ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Location Longitude Deg"));
	m_ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Location Latitude Deg"));
	m_ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Name"));
	m_ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Country"));
	m_ui->tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("T_air + rH"));
	m_ui->tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Radiation"));
	m_ui->tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Wind"));
	m_ui->tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("Pressure"));
	m_ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
	m_ui->tableWidget->horizontalHeader()->setMinimumHeight(40);
	m_ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	//m_ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//m_ui->tableWidget->setst

	m_ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);
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
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item) {
	m_ui->tableWidget->selectRow(item->row() );
	if(item->column() >4){
		// get checked stated, if item was checked, uncheck all others
		if (item->checkState() == Qt::Checked) {
			// prevent events to fire
			m_ui->tableWidget->blockSignals(true);
			for (int i=0; i<m_ui->tableWidget->rowCount(); ++i) {
				int col = item->column();
				QTableWidgetItem *newItem = m_ui->tableWidget->item(i, col);

				if ( newItem == nullptr )
					continue;

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


void MainWindow::on_pushButtonDownload_clicked(){

	//check longitude and latitude
	if(m_ui->lineEditLatitude->text().isEmpty()){
		QMessageBox::critical(this, QString(), "Latitude is empty");
		return;
	}
	if(m_ui->lineEditLongitude->text().isEmpty()){
		QMessageBox::critical(this, QString(), "Longitude is empty");
		return;
	}

	std::vector<int> dataInRows(4,-1);
	//find selected elements
	QTableWidget & tw = *m_ui->tableWidget;
	for (unsigned int row=0; row<tw.rowCount(); ++row) {
		for(unsigned int col=6; col<tw.columnCount(); ++col) {
			if(tw.item(row,col) == nullptr)
				continue;
			if(tw.item(row,col)->checkState() == Qt::Checked) {
				dataInRows[col-6] = IBK::string2val<int>(tw.item(row,0)->text().toStdString());
				//QMessageBox::information(this, QString(), "col: " + QString::number(col) + " | station id: " + QString::number(rows[col-5]) );
			}
		}
	}

	if(dataInRows == std::vector<int>(4,-1)){
		QMessageBox::information(this, "Download Error.", "Download aborted. Please select at least one climate entry e.g. temperature, radiation, ... ");
		return;
	}

	std::vector<QString> filenames(4); //hold filenames for download
	std::vector<DWDData::DataType>	types{DWDData::DT_AirTemperature, DWDData::DT_RadiationDiffuse,
									DWDData::DT_WindDirection,DWDData::DT_Pressure};

	DWDDownloader manager(this);

	manager.m_progress = m_ui->progressBar;
	manager.m_label = m_ui->labelDownload;

	connect( &manager, &DWDDownloader::finished, this, &MainWindow::readData );
	//download the data (zip)

	for(unsigned int i=0; i<4; ++i){
		if(dataInRows[i] != -1){
			DWDData dwdData;
			std::string dateString;
			unsigned int stationId = QString::number(dataInRows[i]).rightJustified(5,'0').toUInt();
			dateString = "_" + m_descDataMap[stationId].m_startDateString + "_" + m_descDataMap[stationId].m_endDateString;

			bool isRecent = m_ui->radioButtonRecent->isChecked();
			QString filename = ""; //only needed by historical

			//only find urls for historical and NO solar data (this is in dataInRows on position 1
			if(!isRecent && i != 1){
				QFtp *ftp = new QFtp;

				connect(ftp, &QFtp::listInfo, &dwdData, &DWDData::findFile );

				ftp->connectToHost("opendata.dwd.de", 21);
				ftp->login("anonymous", "anonymous@opendata.dwd.de");
				ftp->cd("climate_environment");
				ftp->cd("CDC");
				ftp->cd("observations_germany");
				ftp->cd("climate");
				ftp->cd("hourly");


				switch(i){
					case 0:	ftp->cd("air_temperature"); break;
					case 2:	ftp->cd("wind"); break;
					case 3:	ftp->cd("pressure"); break;
				}
				ftp->cd("historical");
				ftp->list();

				while(ftp->hasPendingCommands() || ftp->currentCommand()!=QFtp::None)
					qApp->processEvents();

				while( dwdData.m_urls.empty() )
					qApp->processEvents();

				//now search through all urls for the right file
				for(unsigned int j=0; j<dwdData.m_urls.size(); ++j){
					filename = dwdData.m_urls[j].name();
					//stundenwerte_ST_00183_row.zip
					if((filename.mid(QString("stundenwerte_ST_").length(),5)).toUInt() == stationId){
						qDebug() << "Gefunden: " << filename;
						break;
					}
				}
				dwdData.m_urls.clear();
				qDebug() << "\nclear\n";
			}
			manager.m_urls << dwdData.urlFilename(types[i], QString::number(dataInRows[i]).rightJustified(5,'0'), dateString, isRecent, filename);
			qDebug() << manager.m_urls.back();

			if(isRecent || i==1)
				filenames[i] = dwdData.filename(types[i], QString::number(dataInRows[i]).rightJustified(5,'0'),dateString, isRecent);
			else
				filenames[i] = filename.mid(0, filename.length()-4);
		}
	}
	if(!manager.m_urls.empty())
		QTimer::singleShot(0, &manager, SLOT(execute() ) );

	// dirty way to wait till asynchronous download is finished
	while ( manager.m_isRunning ) // wait for data to be downloaded (the little bit dirty way)
		qApp->processEvents();

	//Check if all downloaded files are valid
	//create a vector with valid files
	std::vector<IBK::Path>	validFiles(4);
	for(unsigned int i=0; i<4; ++i){
		if(dataInRows[i] == -1)
			continue;
		IBK::Path checkfile(std::string(DATA_DIR) + "Tests/" + filenames[i].toStdString() + ".zip");
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
			validFiles[i] = checkfile;
	}

	//open the zip
	//find file with name 'produkt_....'
	//create the file path names and according data types for reading
	std::vector<IBK::Path>	checkedFileNames(4);
	std::map<IBK::Path, std::set<DWDData::DataType>> filenamesForReading;
	for (int i=0; i<filenames.size(); ++i) {
		if ( filenames[i].isEmpty() )
			continue;

		QStringList filesInArchive = JlCompress::getFileList(validFiles[i].str().c_str());
		QStringList filesExtracted;

		QString textFile;

		for ( QString fileName : filesInArchive ) {
			if ( "produkt" == fileName.mid(0,7) ){ // we find the right file
				// we found the file
				textFile = fileName;
				// we extract the file
				QString fileExtracted = JlCompress::extractFile( validFiles[i].str().c_str(), fileName, QString(DATA_DIR) + "Tests/extractedFiles/" + textFile);
				filesExtracted << fileExtracted;
				checkedFileNames[i] = IBK::Path(std::string(DATA_DIR) + "Tests/extractedFiles/" + textFile.toStdString() );
				// was the exraction successful
				if ( fileExtracted.isEmpty() )
					QMessageBox::warning(this, QString(), QString("File %1 could not be extracted").arg(textFile));
			}
		}
	}

	//create extract folder
	for(unsigned int i=0; i<4; ++i){
		if(checkedFileNames[i] == IBK::Path())
			continue; // skip empty states
		switch (i) {
			case 0:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_AirTemperature, DWDData::DT_RelativeHumidity}; break;
			case 1:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_RadiationDiffuse,DWDData::DT_RadiationGlobal, DWDData::DT_RadiationLongWave, DWDData::DT_ZenithAngle}; break;
			case 2:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_WindDirection,DWDData::DT_WindSpeed}; break;
			case 3:
				filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_Pressure}; break;
		}
	}
	//read data

	m_dwdData.m_startTime = IBK::Time(m_ui->lineEditYear->text().toInt(),0);

//	m_progressDlg = new QProgressDialog( tr("Progress"), tr("Abort"), 0, 100, this);
//	m_progressDlg->setWindowModality(Qt::WindowModal);
//	m_progressDlg->setValue(0);
//	m_progressDlg->hide();
//	qApp->processEvents();

//	m_progressDlg->show();

	m_ui->progressBar->setRange(0,100);
	ProgressNotify progressNotify(m_ui->progressBar);

	m_dwdData.m_label = m_ui->labelDownload;
	m_dwdData.createData(&progressNotify, filenamesForReading);
//	m_progressDlg->hide();

	//	dwdData.writeTSV(2001);
	//copy all data in range and create an epw
	double latiDeg = m_ui->lineEditLatitude->text().toDouble();
	double longiDeg = m_ui->lineEditLongitude->text().toDouble();

	///TODO take coordinates from radiation if exists --> zenith angle


	//check data
	for(unsigned int i=0; i<m_dwdData.m_data.size(); ++i){
		DWDData::IntervalData &intVal = m_dwdData.m_data[i];

		if( i == 0 ){
			if(intVal.m_airTemp < -50)
				intVal.m_airTemp = 0;
			if(intVal.m_relHum < 0 || intVal.m_relHum > 100)
				intVal.m_relHum = 50;
			if(intVal.m_counterRad < 0 || intVal.m_counterRad > 1000)
				intVal.m_counterRad = 280;
			if(intVal.m_windDirection < 0 || intVal.m_windDirection > 360)
				intVal.m_windDirection = 0;
			if(intVal.m_windSpeed< 0 || intVal.m_windSpeed> 20)
				intVal.m_windSpeed = 4;
		}
		else{
			//take air temperature of last timepoint
			if(intVal.m_airTemp < -50)
				intVal.m_airTemp = m_dwdData.m_data[i-1].m_airTemp;
			if(intVal.m_relHum < 0 || intVal.m_relHum > 100)
				intVal.m_relHum = m_dwdData.m_data[i-1].m_relHum;
			if(intVal.m_counterRad < 0 || intVal.m_counterRad > 1000)
				intVal.m_counterRad = m_dwdData.m_data[i-1].m_counterRad;
			if(intVal.m_windDirection < 0 || intVal.m_windDirection > 360)
				intVal.m_windDirection = m_dwdData.m_data[i-1].m_windDirection;
			if(intVal.m_windSpeed< 0 || intVal.m_windSpeed> 20)
				intVal.m_windSpeed = m_dwdData.m_data[i-1].m_windSpeed;

		}
		//only radiation
		if(i<=24){
			if(intVal.m_diffRad < 0 || intVal.m_diffRad > 1200)
				intVal.m_diffRad = 0;
			if(intVal.m_globalRad < 0 || intVal.m_globalRad > 1400)
				intVal.m_globalRad = intVal.m_diffRad;			//global must be diffuse radiation if normal radiation is zero
		}
		else{
			if(intVal.m_diffRad < 0 || intVal.m_diffRad > 1200)
				intVal.m_diffRad = m_dwdData.m_data[i-24].m_diffRad;
			if(intVal.m_globalRad < 0 || intVal.m_globalRad > 1400 || intVal.m_globalRad < intVal.m_diffRad)
				intVal.m_globalRad = std::max(m_dwdData.m_data[i-24].m_globalRad, intVal.m_diffRad);
		}
	}


	m_dwdData.exportEPW(m_ui->lineEditYear->text().toInt(), latiDeg, longiDeg);
	QMessageBox::information(this, QString(), "Export done.");
}

void MainWindow::addToList(const QUrlInfo qUrlI){
	m_filelist << qUrlI.name();
}

void MainWindow::on_pushButtonMap_clicked() {
	double latitude = m_ui->lineEditLatitude->text().toDouble();
	double longitude = m_ui->lineEditLongitude->text().toDouble();

	DWDMap::getLocation(latitude, longitude, this);

	m_ui->lineEditLatitude->setText(QString::number(latitude) );
	m_ui->lineEditLongitude->setText(QString::number(longitude) );
	updateTable();
}


void MainWindow::setProgress(int min, int max, int val) {
	FUNCID(setProgress);
	if (m_progressTimer.elapsed() > 100) {
		m_progressDlg->setMaximum(max);
		m_progressDlg->setMaximum(min);
		m_progressDlg->setValue(val);
//		if (m_progressDlg->wasCanceled())
//			throw IBK::Exception("Import canceled.", FUNC_ID);
		m_progressTimer.start();
	}

}

void MainWindow::on_radioButtonHistorical_toggled(bool checked) {
	loadData();
}

void MainWindow::on_lineEditYear_editingFinished() {
	if(m_ui->lineEditYear->isValid()){
		updateTable();
	}

}
