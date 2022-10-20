#include "DWDMainWindow.h"
#include "ui_DWDMainWindow.h"

#include <IBK_NotificationHandler.h>
#include <IBK_messages.h>
#include <IBK_FileReader.h>
#include <IBK_physics.h>
#include <IBK_MessageHandler.h>

#include <QtExt_Directories.h>
#include <QtExt_ValidatingLineEdit.h>

#include <QCheckBox>
#include <QStandardItemModel>
#include <QAbstractTableModel>
#include <QAbstractProxyModel>
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
#include <QDate>
#include <QDebug>

#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <qwt_date_scale_draw.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_grid.h>
#include <qwt_date_scale_engine.h>
#include <qwt_scale_div.h>


#include <qftp.h>

#include <JlCompress.h>

#include "DWDDownloader.h"
#include "DWDTimePlotPicker.h"
#include "DWDMap.h"
// #include "DWDDelegate.h"
#include "DWDData.h"
#include "DWDSortFilterProxyModel.h"
#include "DWDDateTimeScaleEngine.h"
#include "DWDLogWidget.h"
#include "DWDMessageHandler.h"
#include "DWDDelegate.h"
#include "DWDConversions.h"

#include "Constants.h"
#include "Utilities.h"
// #include "DWD_CheckBox.h"



class ProgressNotify : public IBK::NotificationHandler{
public:
	ProgressNotify(QProgressDialog *bar):
		m_dlg(bar)
	{
		m_dlg->setMaximum(100);
		m_dlg->show();
		m_dlg->setMinimumWidth(300);
	}

	/*! Reimplement this function in derived child 'notification' objects
		to provide whatever notification operation you want to perform.
	*/
	virtual void notify() {
		FUNCID(notify);
		if(m_dlg->value() == m_value)
			return;
		m_dlg->setValue(m_value);

		int val = m_dlg->value();
		// qDebug() << m_dlg->value() << " | " << m_dlg->maximum();

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
	m_dwdMap(new DWDMap),
	m_progressDlg(nullptr),
	m_dwdTableModel(new DWDTableModel(this)),
	m_proxyModel(new DWDSortFilterProxyModel(this))
{
	m_ui->setupUi(this);

	m_model = new QStandardItemModel();
	m_ui->radioButtonHistorical->blockSignals(true);
	m_ui->radioButtonHistorical->setChecked(true);
	m_ui->radioButtonHistorical->blockSignals(false);


	m_ui->lineEditDistance->setup(0,1000, "Distance in km", true, true);
	m_ui->lineEditLatitude->setup(-90,90, "Latitude in Deg", true, true);
	m_ui->lineEditLongitude->setup(-180,180, "Longitude in Deg", true, true);
	m_ui->lineEditLatitude->setText("51.03");
	m_ui->lineEditLongitude->setText("13.7");

	//	m_ui->lineEditYear->setup(1950,2023,tr("Year of interest."), true, true);
	//	m_ui->lineEditYear->setText("2020");

	//	on_lineEditYear_editingFinished();

	QTableView * v = m_ui->tableView;
	v->verticalHeader()->setDefaultSectionSize(25);
	v->verticalHeader()->setVisible(false);
	v->horizontalHeader()->setMinimumSectionSize(19);
	v->setSelectionBehavior(QAbstractItemView::SelectRows);
	v->setSelectionMode(QAbstractItemView::SingleSelection);
	v->setAlternatingRowColors(true);
	v->setSortingEnabled(false);
	v->sortByColumn(0, Qt::AscendingOrder);
	// smaller font for entire table
	QFont f;
#ifdef Q_OS_LINUX
	f.setPointSizeF(f.pointSizeF()*0.8);
#endif // Q_OS_WIN
	v->setFont(f);
	v->horizontalHeader()->setFont(f); // Note: on Linux/Mac this won't work until Qt 5.11.1 - this was a bug between Qt 4.8...5.11.1

	m_ui->dateEditStart->setMinimumDate(QDate(1950,1,1));
	m_ui->dateEditStart->setMaximumDate(QDate::currentDate());

	m_ui->dateEditEnd->setMinimumDate(QDate(1950,1,1));
	m_ui->dateEditEnd->setMaximumDate(QDate::currentDate());

	on_horizontalSliderDistance_valueChanged(50);

	m_progressDlg = new QProgressDialog(this);
	// m_progressDlg->setFixedWidth(1000);
	m_progressDlg->setModal(true);

	connect( &m_dwdData, &DWDData::progress, this, &MainWindow::setProgress );

	resize(1500,800);


	// init all plots
	initPlots();

	// Add the Dockwidget
	m_logWidget = new DWDLogWidget;
	this->addDockWidget(Qt::BottomDockWidgetArea, m_logWidget);

	// Also connect all IBK::Messages to Log Widget
	DWDMessageHandler * msgHandler = dynamic_cast<DWDMessageHandler *>(IBK::MessageHandlerRegistry::instance().messageHandler() );
	connect(msgHandler, &DWDMessageHandler::msgReceived, m_logWidget, &DWDLogWidget::onMsgReceived);

	QList<int> sizes;
	sizes << 1500 << 300;
	m_ui->splitter->setSizes(sizes);
}


MainWindow::~MainWindow() {
	delete m_ui;
}

void MainWindow::loadData(){

	// Message
	IBK::IBK_Message("Downloading all weather data from DWD Server.", IBK::MSG_PROGRESS);

	// show progress Dlg
	m_progressDlg->show();
	//download all files
	DWDDescriptonData  descData;

	// get download links for data
	QStringList urls = descData.downloadDescriptionFiles(m_ui->radioButtonRecent->isChecked());

	// initiate download manager
	m_manager = new DWDDownloader(this);
	m_manager->m_urls = urls;
	m_manager->m_progressDlg = m_progressDlg; // bisschen quatsch
	m_progressDlg->
			connect( m_manager, &DWDDownloader::finished, this, &MainWindow::readData );

	m_manager->execute(); // simply registers network requests
}

void MainWindow::setGUIState(bool guiState) {
	m_ui->tableView->setEnabled(guiState);
	m_ui->groupBoxLocation->setEnabled(guiState);
	m_ui->groupBoxTime->setEnabled(guiState);
	m_ui->groupBoxDistance->setEnabled(guiState);
	m_ui->pushButtonPreview->setEnabled(guiState);
	m_ui->pushButtonMap->setEnabled(guiState);
	m_ui->pushButtonDownload->setEnabled(guiState);
}

void MainWindow::downloadData(bool showPreview, bool exportEPW) {
	m_progressDlg->show();

	//check longitude and latitude
	if(m_ui->lineEditLatitude->text().isEmpty()){
		QMessageBox::critical(this, QString(), "Latitude is empty");
		setGUIState(true);
		return;
	}
	if(m_ui->lineEditLongitude->text().isEmpty()){
		QMessageBox::critical(this, QString(), "Longitude is empty");
		setGUIState(true);
		return;
	}

	m_ui->plotTempRelHum->setEnabled(false);
	m_ui->plotRadPres->setEnabled(false);
	m_ui->plotRainWind->setEnabled(false);

	std::vector<int> dataInRows(DWDDescriptonData::NUM_D,-1);
	//find selected elements
	for ( unsigned int i = 0; i<m_descData.size(); ++i ) {
		for ( unsigned int j = 0; j<DWDDescriptonData::NUM_D; ++j ) {
			DWDDescriptonData &dwdData = m_descData[i];

			bool isChecked = dwdData.m_data[j].m_isChecked;

			if (isChecked) {
				dataInRows[j] = i;

				switch (j) {
				case DWDDescriptonData::D_TemperatureAndHumidity:
					m_ui->plotTempRelHum->setEnabled(isChecked);
					break;
				case DWDDescriptonData::D_Solar:
				case DWDDescriptonData::D_Pressure:
					m_ui->plotRadPres->setEnabled(isChecked);
					break;
				case DWDDescriptonData::D_Wind:
				case DWDDescriptonData::D_Precipitation:
					m_ui->plotRainWind->setEnabled(isChecked);
					break;
				}
			}
		}
	}


	if(dataInRows == std::vector<int>(DWDDescriptonData::NUM_D,-1)){
		QMessageBox::information(this, "Download Error.", "Download aborted. Please select at least one climate entry e.g. temperature, radiation, ... ");
		setGUIState(true);
		return;
	}

	std::vector<QString> filenames(DWDDescriptonData::NUM_D); //hold filenames for download
	std::vector<DWDData::DataType>	types{	DWDData::DT_AirTemperature, DWDData::DT_RadiationDiffuse,
				DWDData::DT_WindDirection, DWDData::DT_Pressure, DWDData::DT_Precipitation};

	m_manager = new DWDDownloader(this);
	m_manager->m_progressDlg = m_progressDlg;
	m_manager->m_urls.clear();

	connect( m_manager, &DWDDownloader::finished, this, &MainWindow::readData );
	//download the data (zip)

	for(unsigned int i=0; i<DWDDescriptonData::NUM_D; ++i){
		if(dataInRows[i] != -1){
			DWDData dwdData;
			std::string dateString;

			int idx = dataInRows[i];

			unsigned int stationId = QString::number(m_descData[idx].m_idStation).rightJustified(5,'0').toUInt();
			// find our descData

			dateString = "_" + m_descData[idx].m_startDateString + "_" + m_descData[idx].m_endDateString;

			bool isRecent = m_ui->radioButtonRecent->isChecked();
			QString filename = ""; //only needed by historical

			//only find urls for historical and NO solar data (this is in dataInRows on position 1
			if(!isRecent && i != DWDDescriptonData::D_Solar){
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
				case 4:	ftp->cd("precipitation"); break;
				}
				ftp->cd("historical");
				ftp->list();

				while(ftp->hasPendingCommands() || ftp->currentCommand()!=QFtp::None)
					qApp->processEvents();

				//				while( dwdData.m_urls.empty() )
				//					qApp->processEvents();

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

			m_manager->m_urls << dwdData.urlFilename(types[i], QString::number(m_descData[idx].m_idStation).rightJustified(5,'0'), dateString, isRecent, filename);
			qDebug() << m_manager->m_urls.back();

			if(isRecent || i==1)
				filenames[i] = dwdData.filename(types[i], QString::number(m_descData[idx].m_idStation).rightJustified(5,'0'),dateString, isRecent);
			else
				filenames[i] = filename.mid(0, filename.length()-4);
		}
	}

	IBK::IBK_Message("Start downloading Weather Data...", IBK::MSG_PROGRESS);

	if(!m_manager->m_urls.empty())
		m_manager->execute();

	while ( m_manager->m_isRunning )
		qApp->processEvents();

	//Check if all downloaded files are valid
	//create a vector with valid files
	std::vector<IBK::Path>	validFiles(DWDDescriptonData::NUM_D);
	for(unsigned int i=0; i<DWDDescriptonData::NUM_D; ++i){
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
			case DWDData::DT_Precipitation:		cat = "Precipitation";							break;
			}
			QMessageBox::warning(this, QString(), QString("Download of file '%1' was not successfull. Category: '%2'").arg(filenames[i]+".zip").arg(cat));
		}
		else
			validFiles[i] = checkfile;
	}

	//open the zip
	//find file with name 'produkt_....'
	//create the file path names and according data types for reading
	std::vector<IBK::Path>	checkedFileNames(DWDDescriptonData::NUM_D);
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
	for(unsigned int i=0; i<DWDDescriptonData::NUM_D; ++i){
		if(checkedFileNames[i] == IBK::Path())
			continue; // skip empty states
		switch (i) {
		case DWDDescriptonData::D_TemperatureAndHumidity:
			filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_AirTemperature, DWDData::DT_RelativeHumidity}; break;
		case DWDDescriptonData::D_Solar:
			filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_RadiationDiffuse,DWDData::DT_RadiationGlobal, DWDData::DT_RadiationLongWave, DWDData::DT_ZenithAngle}; break;
		case DWDDescriptonData::D_Wind:
			filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_WindDirection,DWDData::DT_WindSpeed}; break;
		case DWDDescriptonData::D_Pressure:
			filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_Pressure}; break;
		case DWDDescriptonData::D_Precipitation:
			filenamesForReading[checkedFileNames[i]] = std::set<DWDData::DataType>{DWDData::DT_Precipitation}; break;
		}
	}
	//read data

	m_dwdData.m_startTime = DWDConversions::convertQDate2IBKTime(m_ui->dateEditStart->date());

	ProgressNotify progressNotify(m_progressDlg);

	m_dwdData.m_progressDlg = m_progressDlg;
	m_dwdData.createData(&progressNotify, filenamesForReading);

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

	if ( showPreview ) {

		m_ui->plotRadPres->detachItems();
		m_ui->plotRainWind->detachItems();
		m_ui->plotTempRelHum->detachItems();

		// create a new curve to be shown in the plot and set some properties
		QwtPlotCurve *curveTemp = new QwtPlotCurve();
		QwtPlotCurve *curveRelHum = new QwtPlotCurve();
		QwtPlotCurve *curveRad = new QwtPlotCurve();
		QwtPlotCurve *curveWind = new QwtPlotCurve();
		QwtPlotCurve *curvePressure = new QwtPlotCurve();
		QwtPlotCurve *curvePrecipitation = new QwtPlotCurve();

		QColor penColor = DWDDescriptonData::color(DWDDescriptonData::D_TemperatureAndHumidity);
		curveTemp->setPen( penColor, 2 ); // color and thickness in pixels
		curveTemp->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		penColor.setAlpha(123);
		curveRelHum->setPen( penColor, 2 ); // color and thickness in pixels
		curveRelHum->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		curveRad->setPen( DWDDescriptonData::color(DWDDescriptonData::D_Solar), 2 ); // color and thickness in pixels
		curveRad->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		curveWind->setPen( DWDDescriptonData::color(DWDDescriptonData::D_Wind), 2 ); // color and thickness in pixels
		curveWind->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		curvePressure->setPen( DWDDescriptonData::color(DWDDescriptonData::D_Pressure), 2 ); // color and thickness in pixels
		curvePressure->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		curvePrecipitation->setPen( DWDDescriptonData::color(DWDDescriptonData::D_Precipitation), 2 ); // color and thickness in pixels
		curvePrecipitation->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		// data points
		QPolygonF pointsTemp, pointsRelHum, pointsRad, pointsWind, pointsPressure, pointsPrecipitation;


		QDateTime startUTC(m_ui->dateEditStart->date(), QTime(0,0,0,0), Qt::UTC);
		QDateTime startLocal(m_ui->dateEditStart->date(), QTime(0,0,0,0), Qt::LocalTime);

		QDateTime start(m_ui->dateEditStart->date(), QTime(0,0,0,0), Qt::UTC);
		QDateTime end(m_ui->dateEditEnd->date(), QTime(0,0,0,0), Qt::UTC);

		start.setTimeSpec(Qt::UTC);
		end.setTimeSpec(Qt::UTC);


		m_progressDlg->setLabelText("Updating plot charts");
		// Updating plots
		for ( size_t i=0; i<m_dwdData.m_data.size(); ++i ) {
			size_t time = i*3600*1000;

			size_t timeStep = (size_t)start.toMSecsSinceEpoch() /*+ (size_t)60*24*3600*1000*/ + time;
			// qDebug() << time << "\t" << timeStep;

			DWDData::IntervalData intVal = m_dwdData.m_data[i];

			pointsTemp << QPointF(timeStep , intVal.m_airTemp );
			pointsRelHum << QPointF(timeStep , intVal.m_relHum );
			pointsRad << QPointF(timeStep, intVal.m_globalRad );
			pointsWind << QPointF(timeStep, intVal.m_windSpeed );
			pointsPressure << QPointF(timeStep, intVal.m_pressure/1000  ); // in kPa
			pointsPrecipitation << QPointF(timeStep, intVal.m_precipitaion );


			progressNotify.notify((double)(i+1)/m_dwdData.m_data.size() );
		}

		// give some points to the curve
		curveTemp->setSamples(pointsTemp);
		curveRelHum->setSamples(pointsRelHum);
		curveRad->setSamples(pointsRad);
		curveWind->setSamples(pointsWind);
		curvePressure->setSamples(pointsPressure);
		curvePrecipitation->setSamples(pointsPrecipitation);

		// set the curve in the plot
		curveRelHum->attach(m_ui->plotTempRelHum);
		curveRelHum->setYAxis(QwtPlot::yRight);

		curveTemp->attach(m_ui->plotTempRelHum);

		curveRad->attach( m_ui->plotRadPres );
		curveRad->setYAxis(QwtPlot::yRight);

		curvePressure->attach( m_ui->plotRadPres );

		curvePrecipitation->attach( m_ui->plotRainWind );

		curveWind->attach(m_ui->plotRainWind);
		curveWind->setYAxis(QwtPlot::yRight);

		m_ui->plotTempRelHum->replot();
		m_ui->plotTempRelHum->show();

		m_ui->plotRadPres->replot();
		m_ui->plotRadPres->show();

		m_ui->plotRainWind->replot();
		m_ui->plotRainWind->show();
	}

	if ( exportEPW ) {
		QString path = m_ui->lineEditFile->text();
		IBK::Path exportPath(path.toStdString());

		if(!exportPath.isValid()) {
			m_progressDlg->hide();
			QMessageBox::warning(this, "Select EPW file path.", "Please select a file path first for the export of the EPW - file.");
			return;
		}
		m_progressDlg->hide();
		m_dwdData.exportEPW(latiDeg, longiDeg, exportPath);
		QMessageBox::information(this, QString(), "Export done.");
	}


}



void MainWindow::readData() {
	// read all decription files
	DWDDescriptonData descData;
	m_descData.clear();
	IBK::IBK_Message("Reading all weather data from DWD Server.", IBK::MSG_PROGRESS);

	descData.readAllDescriptions(m_descData);

	calculateDistances();


	// we give the data to our table model
	m_dwdTableModel->m_descData = &m_descData;


	m_proxyModel->setSourceModel( m_dwdTableModel );

	m_ui->tableView->setModel(m_proxyModel); // we connect the table model to our view

	m_ui->tableView->setSortingEnabled(true);
	m_ui->tableView->sortByColumn(1, Qt::AscendingOrder);

	m_dwdTableModel->m_proxyModel = m_proxyModel;
	m_ui->tableView->resizeColumnsToContents();


	m_ui->tableView->setItemDelegateForColumn(DWDTableModel::ColPressure, new DWDDelegate);
	m_ui->tableView->setItemDelegateForColumn(DWDTableModel::ColRadiation, new DWDDelegate);
	m_ui->tableView->setItemDelegateForColumn(DWDTableModel::ColAirTemp, new DWDDelegate);
	m_ui->tableView->setItemDelegateForColumn(DWDTableModel::ColPrecipitation, new DWDDelegate);
	m_ui->tableView->setItemDelegateForColumn(DWDTableModel::ColWind, new DWDDelegate);

	m_ui->tableView->setColumnWidth(DWDTableModel::ColPressure, 100);
	m_ui->tableView->setColumnWidth(DWDTableModel::ColAirTemp, 100);
	m_ui->tableView->setColumnWidth(DWDTableModel::ColPrecipitation, 100);
	m_ui->tableView->setColumnWidth(DWDTableModel::ColRadiation, 100);
	m_ui->tableView->setColumnWidth(DWDTableModel::ColWind, 100);

	QHeaderView *headerView = m_ui->tableView->horizontalHeader();
	headerView->setSectionResizeMode(DWDTableModel::ColName, QHeaderView::Stretch);

	m_ui->tableView->reset();

	m_progressDlg->hide();
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
	downloadData(true, true);
}

void MainWindow::addToList(const QUrlInfo qUrlI){
	m_filelist << qUrlI.name();
}

void MainWindow::calculateDistances() {

	double lat1 = m_ui->lineEditLatitude->text().toDouble();
	double lon1 = m_ui->lineEditLongitude->text().toDouble();

	// we calculate for each description the distance to the reference location
	for ( DWDDescriptonData &data : m_descData ) {

		// Latitude & Longitutde
		double lat2 = data.m_latitude;
		double lon2 = data.m_longitude;

		// Calculate distance between two coordinates
		// SOURCE: https://stackoverflow.com/questions/27928/calculate-distance-between-two-latitude-longitude-points-haversine-formula
		double a = 0.5 - cos((lat2-lat1)*IBK::DEG2RAD)/2 + cos(lat1*IBK::DEG2RAD) * cos(lat2*IBK::DEG2RAD) * (1-cos((lon2-lon1)*IBK::DEG2RAD))/2;

		// calc distance
		data.m_distance = 12742 * std::asin(sqrt(a)); //2*R*asin
	}
}

void MainWindow::initPlots() {
	formatQwtPlot(*m_ui->plotTempRelHum, m_ui->dateEditStart->date(), m_ui->dateEditEnd->date(), "Air Temperature & Rel. Humidity", "C", -10, 40, 10, true, "%", 0, 100, 20);
	formatQwtPlot(*m_ui->plotRadPres, m_ui->dateEditStart->date(), m_ui->dateEditEnd->date(), "Pressure & Radiation", "kPa", 0, 1.4, 0.2, true, "W/m2", 0, 1400, 200);
	formatQwtPlot(*m_ui->plotRainWind, m_ui->dateEditStart->date(), m_ui->dateEditEnd->date(), "Precipitation & Wind speed", "mm", 0, 50, 10, true, "m/s", 0, 20, 4);
}

void MainWindow::formatQwtPlot(QwtPlot &plot, QDate startDate, QDate endDate, QString title, QString leftYAxisTitle, double yLeftMin, double yLeftMax, double yLeftStepSize,
							   bool hasRightAxis, QString rightYAxisTitle, double yRightMin, double yRightMax, double yRightStepSize) {

	// initialize start and end date
	QDateTime start(startDate, QTime(0,0,0,0), Qt::UTC);
	QDateTime end(endDate, QTime(0,0,0,0), Qt::UTC);

	// we set also the time spec
	start.setTimeSpec(Qt::UTC);
	end.setTimeSpec(Qt::UTC);

	// initialize all major ticks in grid and Axis
	QList<double> majorTicks;
	majorTicks.push_back(start.toMSecsSinceEpoch() );

	for(unsigned int i=0; i<12; ++i)
		majorTicks.push_back(QwtDate::toDouble(start.addMonths(i) ) );

	// Init Scale Divider
	QwtScaleDiv scaleDiv(QwtDate::toDouble(start), QwtDate::toDouble(end), QList<double>(), QList<double>(), majorTicks);

	// Init Grid
	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->enableXMin(true);
	grid->setXDiv(scaleDiv);
	grid->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
	grid->setMinorPen(QPen(Qt::lightGray, 0 , Qt::DotLine));
	grid->attach(&plot);

	// inti plot title
	QFont font;
	font.setPointSize(10);
	QwtText qwtTitle;
	qwtTitle.setFont(font);
	qwtTitle.setText(title);

	// Set plot title
	plot.setTitle(qwtTitle);

	// Scale all y axises
	plot.setAxisScale(QwtPlot::yLeft, yLeftMin, yLeftMax, yLeftStepSize);
	plot.setAxisFont(QwtPlot::yLeft, font);

	// Init QWT Text
	QwtText axisTitle;
	axisTitle.setFont(font);

	// left Axis title
	axisTitle.setText(leftYAxisTitle);
	plot.setAxisTitle(QwtPlot::yLeft, axisTitle);

	// do we have a right y axis?
	if(hasRightAxis) {
		plot.setAxisFont(QwtPlot::yRight, font);
		plot.enableAxis(QwtPlot::yRight, true);
		plot.setAxisScale(QwtPlot::yRight, yRightMin, yRightMax, yRightStepSize);

		// right Axis title
		axisTitle.setText(rightYAxisTitle);
		plot.setAxisTitle(QwtPlot::yRight, axisTitle);
		plot.setTitle(title);
	}

	// Bottom axis
	plot.setAxisFont(QwtPlot::xBottom, font);

	// Canvas Background
	plot.setCanvasBackground(Qt::white);

	// Init Scale draw engine
	QwtDateScaleDraw *scaleDrawTemp = new QwtDateScaleDraw(Qt::UTC);
	scaleDrawTemp->setDateFormat(QwtDate::Month, "MMM");

	// Set scale draw engine
	plot.setAxisScaleDraw(QwtPlot::xBottom, scaleDrawTemp);
	plot.setAxisScaleDiv(QwtPlot::xBottom, scaleDiv);

	plot.setMinimumHeight(150);
	plot.setMinimumWidth(350);

}


void MainWindow::on_pushButtonMap_clicked() {
	double latitude = m_ui->lineEditLatitude->text().toDouble();
	double longitude = m_ui->lineEditLongitude->text().toDouble();
	unsigned int distance = m_ui->horizontalSliderDistance->value();

	unsigned int year = 2020;
	DWDMap::getLocation(m_descData, latitude, longitude, year, distance, this);

	m_ui->lineEditLatitude->setText(QString::number(latitude) );
	m_ui->lineEditLongitude->setText(QString::number(longitude) );



	calculateDistances();

	//	QModelIndex top = m_dwdTableModel->index(0, 2);
	//	QModelIndex bottom = m_dwdTableModel->index(m_descData.size(), 2);
	//	emit m_dwdTableModel->dataChanged(top, bottom); // we just update the whole column

	m_dwdTableModel->reset();

}


void MainWindow::setProgress(int min, int max, int val) {
	FUNCID(setProgress);

	m_progressDlg->setMaximum(val);

	m_dwdTableModel->reset();
}

void MainWindow::on_radioButtonHistorical_toggled(bool checked) {
	loadData();

	m_ui->tableView->reset();
}


void MainWindow::on_lineEditNameFilter_textChanged(const QString &filter) {
	m_proxyModel->setFilterRegExp(filter);
	m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	m_proxyModel->setFilterKeyColumn(4);
}


void MainWindow::on_horizontalSliderDistance_valueChanged(int value) {
	m_ui->lineEditDistance->setText(QString::number(value) );
	m_proxyModel->setFilterMaximumDistance(value);
	m_proxyModel->setFilterKeyColumn(1);
}

void MainWindow::on_pushButtonPreview_clicked() {
	// init all plots
	initPlots();
	setGUIState(false);
	downloadData(true, false);
	setGUIState(true);
}

void MainWindow::on_toolButtonOpenDirectory_clicked() {

	// request file name
	QString filename = QFileDialog::getSaveFileName(
				this,
				tr("Save EPW File"),
				"",
				tr("EPW Weather file (.epw);;All files (*.*)") );

	if (filename.isEmpty()) return;

	m_fileName = filename;

	m_ui->lineEditFile->setText(filename);
}
