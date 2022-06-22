#include "DWDMainWindow.h"
#include "ui_DWDMainWindow.h"

#include <IBK_NotificationHandler.h>
#include <IBK_messages.h>
#include <IBK_FileReader.h>
#include <IBK_physics.h>

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
#include "DWDProgressBar.h"
#include "DWDDateTimeScaleEngine.h"

#include "Constants.h"
#include "Utilities.h"
// #include "DWD_CheckBox.h"



class ProgressNotify : public IBK::NotificationHandler{
public:
	ProgressNotify( DWDProgressBar *bar):
		m_bar(bar)
	{}

	/*! Reimplement this function in derived child 'notification' objects
		to provide whatever notification operation you want to perform.
	*/
	virtual void notify() {
		FUNCID(notify);
		if(m_bar->value() == m_value)
			return;
		m_bar->setMaximum(m_value);
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

	DWDProgressBar			*m_bar;
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
	m_ui->radioButtonRecent->blockSignals(true);
	m_ui->radioButtonRecent->setChecked(true);
	m_ui->radioButtonRecent->blockSignals(false);


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

	// we fill the comboBox
	for (unsigned int year = 1950; year<2025; year++)
		m_ui->comboBoxYear->addItem(QString::number(year) );

	m_ui->comboBoxYear->setCurrentText("2020");

	on_horizontalSliderDistance_valueChanged(50);

	m_progressDlg = new DWDProgressBar(this);
	connect( &m_dwdData, &DWDData::progress, this, &MainWindow::setProgress );

	resize(1500,800);

	m_ui->checkBoxPressure->setText("Pressure");
	m_ui->checkBoxWind->setText("Wind");
	m_ui->checkBoxTemp->setText("Air Temperature / Relative Humidity");
	m_ui->checkBoxRad->setText("Short-Wave Radiation");
	m_ui->checkBoxRain->setText("Precipitation");

	QList<int> sizes;
	sizes << 1200 << 300;
	m_ui->splitter->setSizes(sizes);

	initPlots();
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

	// show progress Dlg
	m_progressDlg->show();
	//download all files
	DWDDescriptonData  descData;

	m_progressDlg->setTitle("Downloading weather descriptions...");

	// get download links for data
	QStringList urls = descData.downloadDescriptionFiles(m_ui->radioButtonRecent->isChecked());

	// initiate download manager
	m_manager = new DWDDownloader(this);
	m_manager->m_urls = urls;
	m_manager->m_progress = m_progressDlg; // bisschen quatsch
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

	m_ui->plotTemp->setEnabled(false);
	m_ui->plotRad->setEnabled(false);
	m_ui->plotWind->setEnabled(false);
	m_ui->plotPressure->setEnabled(false);
	m_ui->plotRain->setEnabled(false);

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
					m_ui->plotTemp->setEnabled(isChecked);
					break;
				case DWDDescriptonData::D_Solar:
					m_ui->plotRad->setEnabled(isChecked);
					break;
				case DWDDescriptonData::D_Wind:
					m_ui->plotWind->setEnabled(isChecked);
					break;
				case DWDDescriptonData::D_Pressure:
					m_ui->plotPressure->setEnabled(isChecked);
					break;
				case DWDDescriptonData::D_Precipitation:
					m_ui->plotRain->setEnabled(isChecked);
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
	m_manager->m_progress = m_progressDlg;
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

	m_progressDlg->setTitle("Downloading weather data...");

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

	m_dwdData.m_startTime = IBK::Time(m_ui->comboBoxYear->currentText().toInt(),0);

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
		//		m_ui->tempPlot = new QwtPlot( );

		m_ui->plotTemp->detachItems();

		// create plot as main widget
		//m_ui->plotTemp->setAxisScale(QwtPlot::xBottom, 0, 365, 10);


		// create a new curve to be shown in the plot and set some properties
		QwtPlotCurve *curveTemp = new QwtPlotCurve();
		QwtPlotCurve *curveRad = new QwtPlotCurve();
		QwtPlotCurve *curveWind = new QwtPlotCurve();
		QwtPlotCurve *curvePressure = new QwtPlotCurve();
		QwtPlotCurve *curvePrecipitation = new QwtPlotCurve();

		QwtDateScaleDraw *scaleDrawTemp = new QwtDateScaleDraw(Qt::UTC);
		scaleDrawTemp->setDateFormat(QwtDate::Millisecond, "MM.yyyy");
		scaleDrawTemp->setDateFormat(QwtDate::Second, "MM");
		scaleDrawTemp->setDateFormat(QwtDate::Minute, "MM");
		scaleDrawTemp->setDateFormat(QwtDate::Hour, "MM");
		m_ui->plotTemp->setAxisScaleDraw(QwtPlot::xBottom, scaleDrawTemp);
		QwtDateScaleDraw *scaleDrawRad = new QwtDateScaleDraw(Qt::UTC);
		scaleDrawRad->setDateFormat(QwtDate::Millisecond, "MM.yyyy");
		scaleDrawRad->setDateFormat(QwtDate::Second, "MM");
		scaleDrawRad->setDateFormat(QwtDate::Minute, "MM");
		scaleDrawRad->setDateFormat(QwtDate::Hour, "MM");
		m_ui->plotRad->setAxisScaleDraw(QwtPlot::xBottom, scaleDrawRad);
		QwtDateScaleDraw *scaleDrawWind = new QwtDateScaleDraw(Qt::UTC);
		scaleDrawWind->setDateFormat(QwtDate::Millisecond, "MM.yyyy");
		scaleDrawWind->setDateFormat(QwtDate::Second, "MM");
		scaleDrawWind->setDateFormat(QwtDate::Minute, "MM");
		scaleDrawWind->setDateFormat(QwtDate::Hour, "MM");
		m_ui->plotWind->setAxisScaleDraw(QwtPlot::xBottom, scaleDrawWind);
		QwtDateScaleDraw *scaleDrawPressure = new QwtDateScaleDraw(Qt::UTC);
		scaleDrawPressure->setDateFormat(QwtDate::Millisecond, "MM.yyyy");
		scaleDrawPressure->setDateFormat(QwtDate::Second, "MM");
		scaleDrawPressure->setDateFormat(QwtDate::Minute, "MM");
		scaleDrawPressure->setDateFormat(QwtDate::Hour, "MM");
		m_ui->plotPressure->setAxisScaleDraw(QwtPlot::xBottom, scaleDrawPressure);
		QwtDateScaleDraw *scaleDrawRain = new QwtDateScaleDraw(Qt::UTC);
		scaleDrawRain->setDateFormat(QwtDate::Millisecond, "MM.yyyy");
		scaleDrawRain->setDateFormat(QwtDate::Second, "MM");
		scaleDrawRain->setDateFormat(QwtDate::Minute, "MM");
		scaleDrawRain->setDateFormat(QwtDate::Hour, "MM");
		m_ui->plotRain->setAxisScaleDraw(QwtPlot::xBottom, scaleDrawPressure);

		curveTemp->setTitle( "Air Temp" ); // will later be used in legend
		curveTemp->setPen( Qt::blue, 2 ); // color and thickness in pixels
		curveTemp->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		curveRad->setTitle( "SW Rad" ); // will later be used in legend
		curveRad->setPen( Qt::yellow, 2 ); // color and thickness in pixels
		curveRad->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing
		curveRad->setYAxis(QwtPlot::yRight);

		curveWind->setTitle( "Wind" ); // will later be used in legend
		curveWind->setPen( QColor(130, 30, 250), 2 ); // color and thickness in pixels
		curveWind->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		curvePressure->setTitle( "Pressure" ); // will later be used in legend
		curvePressure->setPen( QColor(250, 30, 30), 2 ); // color and thickness in pixels
		curvePressure->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing
		curvePressure->setYAxis(QwtPlot::yRight);

		curvePrecipitation->setTitle( "Precipitation" ); // will later be used in legend
		curvePrecipitation->setPen( QColor(250, 30, 30), 2 ); // color and thickness in pixels
		curvePrecipitation->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing

		// data points
		QPolygonF pointsTemp, pointsRad, pointsWind, pointsPressure, pointsPrecipitation;

		bool ok;
		int year = m_ui->comboBoxYear->currentText().toInt(&ok);

		if(!ok)
			return;
		QDateTime startUTC(QDate(year, 1, 1), QTime(0,0,0,0), Qt::UTC);
		QDateTime startLocal(QDate(year, 1, 1), QTime(0,0,0,0), Qt::LocalTime);

		QDateTime start(QDate(year, 1, 1), QTime(0,0,0,0), Qt::UTC);
		QDateTime end(QDate(year+1, 1, 1), QTime(0,0,0,0), Qt::UTC);

		start.setTimeSpec(Qt::UTC);
		end.setTimeSpec(Qt::UTC);

		for ( size_t i=0; i<m_dwdData.m_data.size(); ++i ) {
			size_t time = i*3600*1000;

			size_t timeStep = (size_t)start.toMSecsSinceEpoch() /*+ (size_t)60*24*3600*1000*/ + time;
			qDebug() << time << "\t" << timeStep;

			DWDData::IntervalData intVal = m_dwdData.m_data[i];

			pointsTemp << QPointF(timeStep , intVal.m_airTemp );
			pointsRad << QPointF(timeStep, intVal.m_globalRad );
			pointsWind << QPointF(timeStep, intVal.m_windSpeed );
			pointsPressure << QPointF(timeStep, intVal.m_pressure );
			pointsPrecipitation << QPointF(timeStep, intVal.m_precipitaion );
		}

		qDebug() << start.toMSecsSinceEpoch() << "\t" << startUTC.toMSecsSinceEpoch() << "\t" << startLocal.toMSecsSinceEpoch();

		//m_ui->plotTemp->setAxisScale(QwtPlot::xBottom, (double)start.toMSecsSinceEpoch(), (double)end.toMSecsSinceEpoch());

		DWDTimePlotPicker * picker = new DWDTimePlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, m_ui->plotTemp->canvas());
		picker->setYUnit("C");
		picker->setTrackerPen(QColor(0,0,64));
		picker->setTrackerMode(QwtPicker::AlwaysOn);
		picker->setStateMachine(new QwtPickerDragRectMachine);

		QList<double> majorTicks;
		majorTicks.push_back(start.toMSecsSinceEpoch() );
		for(unsigned int i=0; i<11; ++i) {
			majorTicks.push_back(start.toMSecsSinceEpoch()+(size_t)dayAtMonthStart[i]*24*60*60*1000);
		}

		QwtScaleDiv scaleDiv((double)start.toMSecsSinceEpoch(), (double)end.toMSecsSinceEpoch(),QList<double>(),QList<double>(),majorTicks);


		QwtPlotGrid *grid = new QwtPlotGrid;
		grid->enableXMin(true);
		grid->setXDiv(scaleDiv);
		grid->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
		grid->setMinorPen(QPen(Qt::lightGray, 0 , Qt::DotLine));
		grid->attach(m_ui->plotTemp);
		grid->attach(m_ui->plotRad);
		grid->attach(m_ui->plotWind);
		grid->attach(m_ui->plotPressure);
		grid->attach(m_ui->plotRain);

		// give some points to the curve
		curveTemp->setSamples( pointsTemp );
		curveRad->setSamples( pointsRad );
		curveWind->setSamples( pointsWind );
		curvePressure->setSamples( pointsPressure );
		curvePrecipitation->setSamples( pointsPrecipitation );

		// set the curve in the plot
		curveTemp->attach( m_ui->plotTemp );
		curveRad->attach( m_ui->plotRad );
		curveWind->attach( m_ui->plotWind );
		curvePressure->attach( m_ui->plotPressure );
		curvePrecipitation->attach( m_ui->plotRain );

		m_ui->plotTemp->replot();
		m_ui->plotTemp->show();

		m_ui->plotRad->replot();
		m_ui->plotRad->show();

		m_ui->plotWind->replot();
		m_ui->plotWind->show();

		m_ui->plotPressure->replot();
		m_ui->plotPressure->show();

		m_ui->plotRain->replot();
		m_ui->plotRain->show();


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
		m_dwdData.exportEPW(m_ui->comboBoxYear->currentText().toInt(), latiDeg, longiDeg, exportPath);
		QMessageBox::information(this, QString(), "Export done.");
	}


}



void MainWindow::readData() {
	// read all decription files
	DWDDescriptonData descData;
	m_descData.clear();
	m_progressDlg->setTitle("Read all weather data...");

	descData.readAllDescriptions(m_descData);

	calculateDistances();


	// we give the data to our table model
	m_dwdTableModel->m_descData = &m_descData;


	m_proxyModel->setSourceModel( m_dwdTableModel );

	m_ui->tableView->setModel(m_proxyModel); // we connect the table model to our view

	m_ui->tableView->setSortingEnabled(true);
	m_ui->tableView->sortByColumn(1, Qt::AscendingOrder);

	m_dwdTableModel->m_proxyModel = m_proxyModel;
	//m_ui->tableView->resizeColumnsToContents();

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


		double lat2 = data.m_latitude;
		double lon2 = data.m_longitude;

		// https://stackoverflow.com/questions/27928/calculate-distance-between-two-latitude-longitude-points-haversine-formula

		double a = 0.5 - cos((lat2-lat1)*IBK::DEG2RAD)/2 + cos(lat1*IBK::DEG2RAD) * cos(lat2*IBK::DEG2RAD) * (1-cos((lon2-lon1)*IBK::DEG2RAD))/2;

		// calc distance
		data.m_distance = 12742 * std::asin(sqrt(a)); //2*R*asin
	}
}

void MainWindow::initPlots() {

	/*! ================= Temp & rel Hum =================== */
	m_ui->plotTemp->setAxisScale(QwtPlot::yLeft, -10, 40, 5);
	m_ui->plotTemp->setAxisTitle(QwtPlot::yLeft, "Temperatur [C]");



	/*! ================= SW Rad =================== */
	m_ui->plotRad->setAxisScale(QwtPlot::yLeft, 0, 1000, 200);
	m_ui->plotRad->setAxisTitle(QwtPlot::yLeft, "SW Radiation [W/m2]");
	m_ui->plotRad->setAxisFont(QwtPlot::yLeft, QFont("Arial", 12));



	/*! ================= Wind =================== */
	m_ui->plotWind->setAxisScale(QwtPlot::yLeft, 0, 40, 5);
	m_ui->plotWind->setAxisTitle(QwtPlot::yLeft, "Wind speed [m/s]");


	/*! ================= Pressure =================== */
	m_ui->plotPressure->setAxisScale(QwtPlot::yLeft, 0, 2, 0.4);
	m_ui->plotPressure->setAxisTitle(QwtPlot::yLeft, "Pressure [kPa]");



	/*! ================= Rain =================== */
	m_ui->plotRain->setAxisScale(QwtPlot::yLeft, 0, 2, 0.4);
	m_ui->plotRain->setAxisTitle(QwtPlot::yLeft, "Pressure [kPa]");




}

void MainWindow::on_pushButtonMap_clicked() {
	double latitude = m_ui->lineEditLatitude->text().toDouble();
	double longitude = m_ui->lineEditLongitude->text().toDouble();

	//	m_dwdMap->setAllDWDLocations(m_descData);
	unsigned int year = m_ui->comboBoxYear->currentText().toUInt();

	DWDMap::getLocation(m_descData, latitude, longitude, year, this);

	m_ui->comboBoxYear->setCurrentText(QString::number(year) );

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

void MainWindow::on_comboBoxYear_currentIndexChanged(const QString &arg1) {
	m_proxyModel->setFilterMinimumDate(QDate(m_ui->comboBoxYear->currentText().toInt(), 1, 1));
	m_proxyModel->setFilterKeyColumn(6);
	m_proxyModel->setFilterMaximumDate(QDate(m_ui->comboBoxYear->currentText().toInt(), 12, 31));
	m_proxyModel->setFilterKeyColumn(7);
}

void MainWindow::on_horizontalSliderDistance_valueChanged(int value) {
	m_ui->lineEditDistance->setText(QString::number(value) );
	m_proxyModel->setFilterMaximumDistance(value);
	m_proxyModel->setFilterKeyColumn(1);
}

void MainWindow::on_pushButtonPreview_clicked() {
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


void MainWindow::on_checkBoxTemp_clicked(bool checked) {
	m_ui->plotTemp->setHidden(!checked);
}

void MainWindow::on_checkBoxRad_clicked(bool checked) {
	m_ui->plotRad->setHidden(!checked);
}

void MainWindow::on_checkBoxRain_clicked(bool checked) {
	m_ui->plotRain->setHidden(!checked);
}

void MainWindow::on_checkBoxPressure_clicked(bool checked) {
	m_ui->plotPressure->setHidden(!checked);
}

void MainWindow::on_checkBoxWind_clicked(bool checked) {
	m_ui->plotWind->setHidden(!checked);
}
