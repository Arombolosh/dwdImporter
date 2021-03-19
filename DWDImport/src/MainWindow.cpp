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

#include "DWDData.h"
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

	testFunc();
}


MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::testFunc(){

	//download all files
	//DWDDescriptonData  descData;


	//descData.downloadDescriptionFiles();

	//read all decription files
	DWDDescriptonData descData;
	std::map<unsigned int, DWDDescriptonData> descDataMap;
	descData.readAllDescriptions(descDataMap);

	//fill table view with these data
	//auto model = m_ui->tableView->model();
	unsigned int counter =0;
	QTableWidget &tw =  *m_ui->tableWidget;
	tw.blockSignals(true);
	for (std::map<unsigned int, DWDDescriptonData>::const_iterator	it = descDataMap.begin();
																	it != descDataMap.end();
																	++it){
		tw.insertRow(counter);
		tw.setItem(counter, 0, new QTableWidgetItem(QString::number(it->second.m_id)));

		tw.setItem(counter, 0, new QTableWidgetItem(QString::number(it->second.m_id)));
		tw.setItem(counter, 1, new QTableWidgetItem(QString::number(it->second.m_longitude)));
		tw.setItem(counter, 2, new QTableWidgetItem(QString::number(it->second.m_latitude)));
		tw.setItem(counter, 3, new QTableWidgetItem(QString::fromStdString(it->second.m_name)));
		tw.setItem(counter, 4, new QTableWidgetItem(QString::fromStdString(it->second.m_country)));

		for (unsigned int i=0;i<5 ; ++i)
			tw.item(counter,i)->setFlags(Qt::ItemIsEnabled);

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
				item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
				item->setCheckState(Qt::Unchecked);
			}
			else
				item->setFlags(Qt::ItemIsEnabled);

			tw.setItem(counter,5+i,item);



//			QStandardItem *item = new QStandardItem(true);
//			CheckBoxHelper *cb = new CheckBoxHelper(counter, 5+i);
//			QWidget *w = new QWidget();

//			QHBoxLayout *lay = new QHBoxLayout();
//			lay->addWidget(cb,0, Qt::AlignHCenter);
//			lay->setMargin(0);
//			w->setLayout(lay);
//			tw.setCellWidget(counter,5+i, w);
//			item->setCheckable(checkable);

//			lay->setEnabled(checkable);

//			connect( cb, &CheckBoxHelper::checkBoxChanged, this, &MainWindow::on_checkboxChecked );

		}

		++counter;
	}
	tw.blockSignals(false);



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



void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item) {
	if(item->column() >4){
		// get checked stated, if item was checked, uncheck all others
		if (item->checkState() == Qt::Checked) {
			// prevent events to fire
			m_ui->tableWidget->blockSignals(true);
			for (int i=0; i<m_ui->tableWidget->rowCount(); ++i) {
				bool test = item->flags() & Qt::ItemIsUserCheckable;
				QMessageBox::information(this, QString(), QString("i: %1 | col: %2 | bool: %3").arg(i).arg(item->column()).arg(test));

				if (i == item->row() && item->flags() & Qt::ItemIsUserCheckable) continue;
				m_ui->tableWidget->item(i, item->column())->setCheckState(Qt::Unchecked);
			}
			m_ui->tableWidget->blockSignals(false);
		}
	}
}



void MainWindow::readDescription(const IBK::Path &filepath, std::map<unsigned int, DWDDescriptonData> &stationDescription, const DWDDescriptonData::Data &dataType){
//	IBK::Path filepath(QtExt::Directories::userDataDir().toStdString() + "filename.txt");

	IBK::FileReader fileReader(filepath);

	//check if file is valid
	if(!fileReader.valid()){
		QMessageBox::warning(this, QString(),QString("File '%1' is not valid").arg(QString::fromStdString(filepath.absolutePath().c_str())));
		return;
	}
	std::vector<std::string> lines;
	//fill lines vector
	fileReader.readAll(filepath, lines,std::vector<std::string>{"\n"});

	//parse each line
	for (unsigned int i=2; i<lines.size(); ++i ) {
		const std::string & line =lines[i];
		if(line.empty())
			break;
		DWDDescriptonData dwd;
		try {
			//extract all informations
			dwd.m_id = IBK::string2val<unsigned int>(line.substr(0,5));
			dwd.m_data[dataType] = 1;

			dwd.m_startDate.set( IBK::string2val<unsigned int>(line.substr(6,4)), IBK::string2val<unsigned int>(line.substr(10,2))-1,IBK::string2val<unsigned int>(line.substr(12,2))-1,0);
			dwd.m_endDate.set( IBK::string2val<unsigned int>(line.substr(15,4)), IBK::string2val<unsigned int>(line.substr(19,2))-1,IBK::string2val<unsigned int>(line.substr(21,2))-1,0);
			dwd.m_height = IBK::string2val<double>(line.substr(24,38-24));
			dwd.m_latitude = IBK::string2val<double>(line.substr(39,50-39));
			dwd.m_longitude = IBK::string2val<double>(line.substr(51,60-51));
			dwd.m_name = IBK::trim_copy(line.substr(61,100-61));
			dwd.m_country = IBK::trim_copy(line.substr(101,500));

//			QMessageBox::warning(this, QString(),
//								 QString("%1\n").arg(dwd.m_id)
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_startDate.toDateTimeFormat()))
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_endDate.toDateTimeFormat()))
//								 + QString("%1\n").arg(dwd.m_height)
//								 + QString("%1\n").arg(dwd.m_latitude)
//								 + QString("%1\n").arg(dwd.m_longitude)
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_name))
//								 + QString("%1\n").arg(QString::fromStdString(dwd.m_country))
//								 + QString("%1\n").arg(QString::fromStdString(line))
//								 );
			//add dwd object to map
			///TODO check if other information are equal
			if(stationDescription.find(dwd.m_id) != stationDescription.end())
				stationDescription[dwd.m_id].m_data[dataType] = 1;
			else
				stationDescription[dwd.m_id] = dwd;
		}  catch (IBK::Exception &ex) {
			QMessageBox::warning(this, QString(), QString("Got an exception while reading lines. In line %1\n").arg(i)
								 + QString("%1\n").arg(dwd.m_id)
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_startDate.toDateTimeFormat()))
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_endDate.toDateTimeFormat()))
								 + QString("%1\n").arg(dwd.m_height)
								 + QString("%1\n").arg(dwd.m_latitude)
								 + QString("%1\n").arg(dwd.m_longitude)
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_name))
								 + QString("%1\n").arg(QString::fromStdString(dwd.m_country))
								 + QString("%1\n").arg(QString::fromStdString(line))
								 );
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

#if 0

void MainWindow::on_radioButton_ShapeI_toggled(bool checked)
{
		m_ui->doubleSpinBox_Len1->setEnabled(!checked);
		m_ui->doubleSpinBox_Wid1->setEnabled(!checked);
		m_ui->doubleSpinBox_Len2->setEnabled(!checked);
		m_ui->doubleSpinBox_Wid2->setEnabled(!checked);

		QPixmap shapePic;
		if (shapePic.load("C:/Temp/test.png"))
			m_ui->labelShapeView->setPixmap(shapePic);
}

void MainWindow::on_radioButton_ShapeL_toggled(bool checked)
{
		m_ui->doubleSpinBox_Len2->setEnabled(!checked);
		m_ui->doubleSpinBox_Wid2->setEnabled(!checked);
}

void MainWindow::on_radioButton_oneWing_toggled(bool checked)
{
	m_isOneWing = checked;
}



void MainWindow::on_pushButton_CreateBuilding_clicked()
{
	try {



	double maxDistanceClipping = m_ui->doubleSpinBoxMaxClipDist->value();		// max clipping distance in m

	//check filename
	QString filename=m_ui->lineEdit_OutputFile->text();
	if(filename.isEmpty()){
		QMessageBox::critical(this, QString(), QMessageBox::tr("Filename is empty. Please insert a filename."));
		return;
	}
	//check output folder
	QDir filepath(m_ui->lineEdit_OutputFolder->text());
	if(!filepath.exists() ){
		QMessageBox::critical(this, QString(), QMessageBox::tr("No valid folder. Please select a folder for file."));
		return;
	}
	//create filepath
	IBK::Path outfile(filepath.path().toStdString());
	IBK::Path test02 ((filename + ".idf").toStdString());
	if(m_ui->radioButton_ExportIDF->isChecked())
		outfile = IBK::Path(filepath.path().toStdString()) / IBK::Path((filename + ".idf").toStdString());
	//else
	//outfile + (filename + ".nandrad").toStdString();
	std::string test01 = outfile.str();
	GB::Building building;

	if(m_progressDlg == nullptr)
		m_progressDlg = new QProgressDialog(tr("Clipping ..."), tr("Abbrechen"),0,/*max*/100, this);


	m_progressDlg->setLabelText("Clipping ...");
	m_progressDlg->setValue(0);
	//m_progressDlg->setWindowModality(Qt::WindowModal);
	m_progressDlg->show();

	ProgressNotify notify(m_progressDlg);

	NSG::Polygon::writeLine("Neuer Versuch");

	//decide on tabwidget index
	//only cutting operation
	if(m_ui->tabWidget->currentIndex() == 2){


		//check output folder
		QFile filepathIDF(m_ui->lineEdit_InputFileIDF->text());
		if(!filepathIDF.exists() ){
			QMessageBox::critical(this, QString(), QMessageBox::tr("No valid input file. Please select a valid file."));
			return;
		}
		IBK::Path filepathIDF2(m_ui->lineEdit_InputFileIDF->text().toStdString());
		//read idf
		m_progressDlg->setLabelText("Import IDF-File ...");
		building.importIDF(filepathIDF2, &notify);
		m_progressDlg->setLabelText("Clipping ...");
		m_progressDlg->setValue(0);
		building.m_buildingType = GB::Building::BT_UserClippingBuilding;

		//clipping
		GB::Clipping clip;
		//u can set up the maximum clipping distance
		clip.clip(building, &notify, maxDistanceClipping);
	}
	//create building and cutting operation
	else {
		//transfer input values to window model
		GB::Surface::WindowParameterForAutoCreate winPara;
		winPara.m_setHeight = m_ui->doubleSpinBox_WindowHeight->value();
		winPara.m_setLength = m_ui->doubleSpinBox_WindowWidth->value();
		winPara.m_setSillHeight = m_ui->doubleSpinBox_WindowSillHeight->value();
		winPara.m_setDistance= m_ui->doubleSpinBox_DistanceWindowWIndow->value();

		//check inputs
		if(winPara.m_setHeight == 0 || winPara.m_setLength == 0 || winPara.m_setSillHeight == 0 || winPara.m_setDistance == 0)
			QMessageBox::critical(this, tr("Window parameter"), QMessageBox::tr("Invalid window parameter. Please change parameter."));

		//create building
		if(m_ui->tabWidget->currentIndex() == 0){
			createOfficeBuilding(building);
			building.m_buildingType = GB::Building::BT_Office;
		}
		else if (m_ui->tabWidget->currentIndex() == 1) {
			createResidential(building);
			building.m_buildingType = GB::Building::BT_Residential;
		}
		else {
			//todo fehler ...
		}


		try {
			IBK::Path testOutFile(outfile);
			testOutFile = testOutFile.withoutExtension() +"OutFileTest.idf";
			building.writeIDF(testOutFile);
		}
		catch (IBK::Exception &ex) {

		}
		catch(std::exception &ex){

		}

		//proceed with clipping
		GB::Clipping::clip(building, &notify, maxDistanceClipping);
		//create windows
		//window parameter by user inputs ToDo
		GB::Clipping::autoWindowGeneration(building, winPara);
	}

	//calculate shading
	//todo later


	m_progressDlg->setLabelText("Write IDF-File ...");
	m_progressDlg->setValue(0);

	try {
		building.writeIDF(outfile, &notify);
	} catch (IBK::Exception &ex) {

	}
	catch(std::exception &ex){

	}

//	m_progressDlg->close();
	m_progressDlg->hide();
	QMessageBox::information(this, tr("Create Building"), QMessageBox::tr("Finished generic Building"));
	} catch (IBK::Exception &ex) {
		QString str = ex.what();
	}
	catch (std::exception &ex) {
		QString str = ex.what();
	}
}

void MainWindow::on_pushButton_OutputFile_clicked()
{
	QString fpath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),QString(),QFileDialog::ShowDirsOnly);
	if(fpath.isEmpty())
		return;
	m_ui->lineEdit_OutputFolder->setText(fpath);
}


void MainWindow::on_pushButtonClose_clicked()
{
	//eine große Fläche wird mit einer kleinen Fläche verschnitten. Dabei würde die kleine Fläche ein Loch in die große Fläche schneiden.
	NSG::SurfaceClipping surf;
	/*

	std::vector<IBKMK::Vector3D>	polyline01, polyline02;

	polyline01.push_back(IBKMK::Vector3D(0,0,0));
	polyline01.push_back(IBKMK::Vector3D(10,0,0));
	polyline01.push_back(IBKMK::Vector3D(10,10,0));
	polyline01.push_back(IBKMK::Vector3D(0,10,0));

	polyline02.push_back(IBKMK::Vector3D(3,6,0.1));
	polyline02.push_back(IBKMK::Vector3D(6,6,0.1));
	polyline02.push_back(IBKMK::Vector3D(6,3,0.1));
	polyline02.push_back(IBKMK::Vector3D(3,3,0.1));

	surf.addPolygon(polyline02, "Small");
	surf.addPolygon(polyline01, "Big");

	surf.clipping();
	*/

	//surf.testDirk001();

	close();
}

void MainWindow::createOfficeBuilding(GB::Building &building)
{
	//set building dimensions
	m_outsideDimensions.resize(3);
	m_outsideDimensions[0].m_height = m_ui->doubleSpinBox_Height->value();
	m_outsideDimensions[0].m_length = m_ui->doubleSpinBox_Len0->value();
	m_outsideDimensions[0].m_width = m_ui->doubleSpinBox_Wid0->value();
	m_outsideDimensions[1].m_length = m_ui->doubleSpinBox_Len1->value();
	m_outsideDimensions[1].m_width = m_ui->doubleSpinBox_Wid1->value();
	m_outsideDimensions[2].m_length = m_ui->doubleSpinBox_Len2->value();
	m_outsideDimensions[2].m_width = m_ui->doubleSpinBox_Wid2->value();

	if(m_ui->radioButton_ShapeI->isChecked()){
		building.defineOutsideDimension(m_outsideDimensions[0].m_length, m_outsideDimensions[0].m_width, m_outsideDimensions[0].m_height, m_isOneWing);
		building.createSingleBuildingBlocks();
	}
	else if (m_ui->radioButton_ShapeL->isChecked()) {
		building.defineOutsideDimension(m_outsideDimensions[0].m_length, m_outsideDimensions[0].m_width, m_outsideDimensions[0].m_height, m_isOneWing,
				m_outsideDimensions[1].m_length, m_outsideDimensions[1].m_width);
		building.createLShapeBuildingBlocks();
	}
	else if (m_ui->radioButton_ShapeU->isChecked()) {
		building.defineOutsideDimension(m_outsideDimensions[0].m_length, m_outsideDimensions[0].m_width, m_outsideDimensions[0].m_height, m_isOneWing,
				m_outsideDimensions[1].m_length, m_outsideDimensions[1].m_width,
				m_outsideDimensions[2].m_length, m_outsideDimensions[2].m_width);
		building.createUShapeBuildingBlocks();
	}
}

void MainWindow::createResidential(GB::Building & building)
{
	GB::Residential resi;
	resi.m_outsideDimensions.m_width = m_ui->doubleSpinBoxResiWidth->value();
	resi.m_outsideDimensions.m_length = m_ui->doubleSpinBoxResiLength->value();
	resi.m_outsideDimensions.m_height = m_ui->doubleSpinBoxResiHeight->value();

	bool calcWithMinHeight=true;
	//check minimum parameter for gui
	if(m_ui->radioButtonStoreyHeight->isChecked()){
		resi.setHeight(m_ui->doubleSpinBoxApartmentHeight->value());
		resi.setNumberOfStoreys(m_ui->doubleSpinBoxNumberOfStoreys->value());
		calcWithMinHeight = false;
	}
	resi.calc(building, static_cast<GB::Residential::RoomPlanNumber>(m_ui->comboBoxFloorPlanResidential->currentIndex()),calcWithMinHeight);
}

void testSurfaces(GB::Building building){

	std::map<size_t, unsigned int>		surfaceIdToCount;
	std::map<size_t, unsigned int>		linkedSurfaceIdToCount;


	for(auto room : building.m_rooms){
		for(auto surf : room.m_surfaces){
			if(surfaceIdToCount.find(surf.m_id) != surfaceIdToCount.end())
				++surfaceIdToCount[surf.m_id];
			else {
				surfaceIdToCount[surf.m_id] = 0;
			}

			if(surf.m_interfaceBoundary == GB::Surface::IB_Surface){
				if(linkedSurfaceIdToCount.find(surf.m_connectedId) != linkedSurfaceIdToCount.end())
					++linkedSurfaceIdToCount[surf.m_connectedId];
				else {
					linkedSurfaceIdToCount[surf.m_connectedId] = 0;
				}

			}
		}
	}
}

void MainWindow::testCaseMerge(){


	GB::Building building;
	building.m_buildingType = GB::Building::BT_Residential;
	GB::Residential resi;
	resi.m_outsideDimensions.m_width = 6.1;
	resi.m_outsideDimensions.m_length = 20.25;
	resi.m_outsideDimensions.m_height = 3;
	resi.setHeight(3);
	resi.setNumberOfStoreys(3);
	resi.calc(building, GB::Residential::RP_1, false);


	double maxDistanceClipping = 0.8;
	//proceed with clipping
	GB::Clipping::clip(building, nullptr, maxDistanceClipping);
	//create windows
	//window parameter by user inputs ToDo

	testSurfaces(building);
	GB::Surface::WindowParameterForAutoCreate winPara;
	GB::Clipping::autoWindowGeneration(building, winPara);

	EP::Project prj = building.createProject(nullptr);

	std::map<std::string,unsigned int>	namesToCount, namesToCount2;
	for (auto e : prj.m_bsd) {
		unsigned int count = namesToCount[e.m_name];
		++count;
		if(count>1)
		{
			double x=0;
		}
		namesToCount[e.m_name] = count;
		if(e.m_outsideBoundaryCondition == EP::BuildingSurfaceDetailed::OC_Surface){
			unsigned int count2 = namesToCount2[e.m_outsideBoundaryConditionObject];
			++count2;
			namesToCount2[e.m_outsideBoundaryConditionObject]=count2;
			if(count2>1)
			{
				double x=0;
			}
		}
	}
	return;

	EP::Project prjMerge;

	for (size_t i=1; i<=15; ++i) {
		EP::Project prj;
		prj.readIDF(IBK::Path("c:/temp/variantenStudieGebIdx" + IBK::val2string(i) + ".idf"));

		prjMerge = prjMerge.mergeProjects(prj, IBKMK::Vector3D(0,i*30,0));


		std::map<std::string,unsigned int>	namesToCount, namesToCount2;
		for (auto e : prj.m_bsd) {
			unsigned int count = namesToCount[e.m_name];
			++count;
			if(count>1)
			{
				double x=0;
			}
			namesToCount[e.m_name] = count;
			if(e.m_outsideBoundaryCondition == EP::BuildingSurfaceDetailed::OC_Surface){
				unsigned int count2 = namesToCount2[e.m_outsideBoundaryConditionObject];
				++count2;
				namesToCount2[e.m_outsideBoundaryConditionObject]=count2;
				if(count2>1)
				{
					double x=0;
				}
			}
		}
	}

}


void MainWindow::testCaseBuilding()
{
	testCaseMerge();

	FUNCID(MainWindow::testCaseBuilding);
	if(m_progressDlg == nullptr)
		m_progressDlg = new QProgressDialog(tr("Clipping ..."), tr("Abbrechen"),0,/*max*/100, this);

	m_progressDlg->setLabelText("Clipping ...");
	m_progressDlg->setValue(0);
	//m_progressDlg->setWindowModality(Qt::WindowModal);
	m_progressDlg->show();

	ProgressNotify notify(m_progressDlg);

	unsigned int maxIdx = 0;
	double shiftLen = 20;
	double stepsizeWidth = 0.5;
	double stepsizeLen = 1;
	double maxLen = 30;
	EP::Project mergePrj;

	unsigned int counter=0;

	unsigned int maxSize = ((int)GB::Residential::NUM_RP+1)*30*60;
	m_progressDlg->setMaximum(maxSize);

	try {

	for (size_t iRpNr = 0; iRpNr<GB::Residential::NUM_RP; ++iRpNr) {
		IBKMK::Vector3D shift(20,0,0);
		m_progressDlg->setLabelText(QString("Progress dialog for floor plan number %1").arg(iRpNr));
		//roomplan
		GB::Residential::RoomPlanNumber rpNr = static_cast<GB::Residential::RoomPlanNumber>(iRpNr);
		std::map<size_t, GB::ConstructionType>	constrMap;
		GB::Residential resi;
		resi.setMinParaForRoomPlan(rpNr, constrMap, false, true);
		double currentWidth = resi.m_minWidth;
		double currentLength = resi.m_minLength;
		unsigned int idx =0;
		double maxWidth = resi.m_maxWidth;
		m_progressDlg->setMaximum((resi.m_maxWidth-resi.m_minWidth)/stepsizeWidth * (maxLen - resi.m_minLength)/stepsizeLen);
		double testMax = m_progressDlg->maximum();
		//loop for current witdh
		while(currentWidth <= maxWidth){
			//dimensions
//			resi.setMinParaForRoomPlan(rpNr, constrMap, false, true);
			//loop for current length
			currentLength = resi.m_minLength;
			while(currentLength<=maxLen){
				IBK::IBK_Message(IBK::FormatString("\nBuilding number %1\n").arg(counter),IBK::MSG_PROGRESS, FUNC_ID);
				++counter;

				unsigned int maxVal = (iRpNr+1)*
						(int)((currentWidth-resi.m_minWidth)/stepsizeWidth)*
						(int)((currentLength-resi.m_minLength)/stepsizeLen);
				m_progressDlg->setValue(idx + iRpNr*maxIdx);
				GB::Building building;
				building.m_buildingType = GB::Building::BT_Residential;
				resi = GB::Residential();
				resi.m_outsideDimensions.m_width = currentWidth;
				resi.m_outsideDimensions.m_length = currentLength;
				resi.m_outsideDimensions.m_height = 3;
				resi.setHeight(3);
				resi.setNumberOfStoreys(3);
				resi.calc(building, rpNr, false);

				double maxDistanceClipping = 0.8;
				//proceed with clipping
				GB::Clipping::clip(building, &notify, maxDistanceClipping);
				//create windows
				//window parameter by user inputs ToDo
				GB::Surface::WindowParameterForAutoCreate winPara;
				GB::Clipping::autoWindowGeneration(building, winPara);

				EP::Project prj = building.createProject(&notify);
				//if(counter == 14)
					prj.writeIDF(IBK::Path("c:/temp/variantenStudieGebIdx" + IBK::val2string(counter) + ".idf"));
				if(mergePrj.m_zones.empty())
					mergePrj = prj;
				else{
					shift = IBKMK::Vector3D(idx*20, iRpNr * 150 , 0);
					mergePrj = mergePrj.mergeProjects(prj, shift);
				}
				//shift.m_y += shiftLen;

				currentLength += stepsizeLen;
				++idx;
				if(idx > maxIdx)
					maxIdx = idx;
			}
			currentWidth += stepsizeWidth;
		}
		mergePrj.writeIDF(IBK::Path("c:/temp/variantenStudie" + IBK::val2string(iRpNr) + ".idf"));
		mergePrj = EP::Project();
	}

	} catch (IBK::Exception &ex) {
		IBK::IBK_Message(IBK::FormatString("Program crashed.\n %1").arg(ex.what()), IBK::MSG_ERROR, FUNC_ID);
	}

}



void MainWindow::on_comboBoxFloorPlanResidential_currentIndexChanged(int index)
{
	GB::Building b1;
	GB::Residential resi;
	resi.setMinParaForRoomPlan(static_cast<GB::Residential::RoomPlanNumber>(index), b1.m_constructions, false);
	//for construction thickness use standard parameter at this time
	//todo if user constructions are available adjust this functionality
	resi.setStandardParameter(b1.m_constructions);
	m_ui->doubleSpinBoxResiWidth->setRange(resi.m_minWidth+2*resi.m_constructionsThickness[GB::Residential::CT_OutdoorWall], resi.m_maxWidth+2*resi.m_constructionsThickness[GB::Residential::CT_OutdoorWall]);
	m_ui->doubleSpinBoxResiLength->setRange(resi.m_minLength+2*resi.m_constructionsThickness[GB::Residential::CT_OutdoorWall], 500);

}

void MainWindow::on_radioButtonStoreyHeight_toggled(bool checked)
{
	m_ui->doubleSpinBoxApartmentHeight->setEnabled(checked);
	m_ui->doubleSpinBoxNumberOfStoreys->setEnabled(checked);
	m_ui->doubleSpinBoxResiHeight->setEnabled(!checked);

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	//Clipping Mode
	if(index==2){
		m_ui->groupBoxWindowParameter->setVisible(false);
		m_ui->pushButton_CreateBuilding->setText("Clipping");
	}
	else {
		m_ui->groupBoxWindowParameter->setVisible(true);
		m_ui->pushButton_CreateBuilding->setText("Create Building");
	}

}

void MainWindow::on_pushButton_InputFileIDF_clicked()
{
	QString fpath = QFileDialog::getOpenFileName(this, tr("Open File"),QString(),"*.idf");
	if(fpath.isEmpty())
		return;
	m_ui->lineEdit_InputFileIDF->setText(fpath);
}

void MainWindow::on_pushButton_Test_clicked()
{
	testCaseBuilding();
	return;
	//Test mergen von zwei Projekten
	EP::Project prj1, prj2, mergePrj;
	prj1.readIDF(IBK::Path("c:/temp/residential01.idf"));
	prj2.readIDF(IBK::Path("c:/temp/residential02.idf"));
	//mergePrj = prj1;
	mergePrj = prj1.mergeProjects(prj2, IBKMK::Vector3D(100,0,0));
	mergePrj.writeIDF(IBK::Path("c:/temp/ResiMerge01.idf"));
	return;

	IFC::Classes ifc;
	IBK::Path filename(m_ui->lineEdit_InputIFC->text().toStdString());
	ifc.read(filename);
	ifc.process(filename);
}

void MainWindow::on_pushButton_InputIFC_clicked()
{
	QString fpath = QFileDialog::getOpenFileName(this, tr("Open File"),QString(),"*.ifc");
	if(fpath.isEmpty())
		return;
	m_ui->lineEdit_InputIFC->setText(fpath);
}

#endif

