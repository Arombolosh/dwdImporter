#include "DWDMap.h"
#include "ui_DWDMap.h"
#include "DWDScene.h"

#include "DWDDescriptonData.h"

#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QString>

QDate convertIBKTimeToQDate2(const IBK::Time &time) {

	int y;
	unsigned int m, d;
	double s;

	time.decomposeDate(y, m, d, s);

	return QDate(y, m+1, d+1);

};

void getPosition(QSize size, const double &longitude, const double &latitude, double &xPos, double &yPos) {
	int width = size.width();
	int height = size.height();

	yPos = height - ((latitude - 47.271679) / ( 55.05864 - 47.271679 ))*height;
	xPos = width + ((15.043611 - longitude) / ( 5.866944 - 15.043611 ))*width;
}

void drawLocationInformation(QGraphicsItemGroup *group, int xPos, int yPos, int rad, int alpha, QColor color) {
	//	scene->addEllipse(xPos-rad, yPos-rad, rad*2.0, rad*2.0,
	//							QPen(color), QBrush(color, Qt::SolidPattern));

	QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(xPos-rad/2, yPos-rad/2, rad, rad);
	//	>ellipse->setPen(QPen (color, 2, Qt::SolidLine));
	color.setAlpha(alpha);
	ellipse->setBrush(QBrush (color) );

	group->addToGroup(ellipse);

	//	scene->addEllipse(xPos-rad, yPos-rad, rad, rad, QPen(color, 2, Qt::SolidLine), QBrush() );
}

DWDMap::DWDMap(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::DWDMap)
{

	m_ui->setupUi(this);

	m_scene = new DWDScene(this);


	QGraphicsView *view = m_ui->graphicsViewMap;
	view->setScene(m_scene);

	m_scene->addPixmap( QPixmap(":/gfx/Karte_Deutschland.svg").scaled(1200,1500,Qt::KeepAspectRatio,Qt::SmoothTransformation) );
	QList<QGraphicsItem*> list = m_scene->items();
	list.at(0)->setZValue(-100);

	double width = m_scene->width();
	double height = m_scene->height();

	m_ui->checkBoxAirTemp->setText("Air Temperature / Relative Humidity");
	m_ui->checkBoxRadiation->setText("Short-Wave Radiation");
	m_ui->checkBoxPressure->setText("Pressure");
	m_ui->checkBoxWind->setText("Wind Speed");
	m_ui->checkBoxPrecipitation->setText("Precipitation");

	//	blockSignals(true);
	//	m_ui->checkBoxAirTemp->setChecked(true);
	//	m_ui->checkBoxRadiation->setChecked(true);
	//	m_ui->checkBoxPressure->setChecked(true);
	//	m_ui->checkBoxWind->setChecked(true);
	//	blockSignals(false);

	m_ui->comboBoxYear->blockSignals(true);
	// we fill the comboBox
	for (unsigned int year = 1950; year<2025; year++)
		m_ui->comboBoxYear->addItem(QString::number(year) );
	m_ui->comboBoxYear->setCurrentIndex(2020-1950);
	m_ui->comboBoxYear->blockSignals(false);

	m_ui->horizontalSliderDiameter->blockSignals(true);
	m_ui->horizontalSliderDiameter->setValue(30);
	m_ui->horizontalSliderDiameter->blockSignals(false);


	m_ui->horizontalSliderOpacity->blockSignals(true);
	m_ui->horizontalSliderOpacity->setValue(255);
	m_ui->horizontalSliderOpacity->blockSignals(false);


	view->setFixedSize(width, height);
	view->setSceneRect(0, 0, width, height);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_color[DWDDescriptonData::D_Wind] = QColor ("#008b8b");
	m_color[DWDDescriptonData::D_TemperatureAndHumidity] = QColor ("#ffa500");
	m_color[DWDDescriptonData::D_Solar] = QColor ("#00ff00");
	m_color[DWDDescriptonData::D_Pressure] = QColor ("#0000ff");
	m_color[DWDDescriptonData::D_Precipitation] = QColor ("#ff1493");

	m_ui->checkBoxAirTemp->setStyleSheet("QCheckBox { color: #ffa500 }");
	m_ui->checkBoxRadiation->setStyleSheet("QCheckBox { color: #00ff00 }");
	m_ui->checkBoxPressure->setStyleSheet("QCheckBox { color: #0000ff }");
	m_ui->checkBoxWind->setStyleSheet("QCheckBox { color: #008b8b }");
	m_ui->checkBoxPrecipitation->setStyleSheet("QCheckBox { color: #ff1493 }");

	m_descTypeToDraw[DWDDescriptonData::D_Wind] = true;
	m_descTypeToDraw[DWDDescriptonData::D_TemperatureAndHumidity] = true;
	m_descTypeToDraw[DWDDescriptonData::D_Solar] = true;
	m_descTypeToDraw[DWDDescriptonData::D_Pressure] = true;
	m_descTypeToDraw[DWDDescriptonData::D_Precipitation] = true;

	m_items[DWDDescriptonData::D_Wind] = new QGraphicsItemGroup();
	m_items[DWDDescriptonData::D_TemperatureAndHumidity] = new QGraphicsItemGroup();
	m_items[DWDDescriptonData::D_Solar] = new QGraphicsItemGroup();
	m_items[DWDDescriptonData::D_Pressure] = new QGraphicsItemGroup();
	m_items[DWDDescriptonData::D_Precipitation] = new QGraphicsItemGroup();

	m_scene->addItem(m_items[DWDDescriptonData::D_Wind]);
	m_scene->addItem(m_items[DWDDescriptonData::D_Solar]);
	m_scene->addItem(m_items[DWDDescriptonData::D_TemperatureAndHumidity]);
	m_scene->addItem(m_items[DWDDescriptonData::D_Pressure]);
	m_scene->addItem(m_items[DWDDescriptonData::D_Precipitation]);
}

DWDMap::~DWDMap() {
	delete m_ui;
}

void DWDMap::mouseMoveEvent(QMouseEvent * event) {
	// we get the position of the mouse

}

void DWDMap::mousePressEvent(QMouseEvent *e) {

	double rad = 5;
	QPointF pt = m_ui->graphicsViewMap->mapToScene(e->pos());

	QList<QGraphicsItem*> list = m_scene->items();
	if ( list.size()>2 )
		m_scene->removeItem( list.at(0) );

	m_scene->addEllipse(pt.x()-rad-18, pt.y()-rad-18, rad*2.0, rad*2.0,
						QPen(), QBrush(Qt::SolidPattern));

	m_latitude = m_scene->m_latitude;
	m_longitude = m_scene->m_longitude;

	// set in lineEditFields
	m_ui->lineEditLatitude->setText( QString::number(m_scene->m_latitude) );
	m_ui->lineEditLongitude->setText( QString::number(m_scene->m_longitude) );
}

void DWDMap::setLocation(const double &latitude, const double &longitude) {
	m_latitude = latitude;
	m_longitude = longitude;

	double xPos;
	double yPos;

	m_size = m_ui->graphicsViewMap->maximumViewportSize();

	getPosition(m_size, m_longitude, m_latitude, xPos, yPos);

	double rad = 5;

	//	drawWeather(m_scene, xPos, yPos, 8, Qt::black);

	m_ui->lineEditLatitude->setText(QString::number(m_latitude));
	m_ui->lineEditLongitude->setText(QString::number(m_longitude));
}

void DWDMap::setYear(const unsigned int & year) {
	m_year = year;
	drawAllDataForYear(m_year);
}

void DWDMap::setAllDWDLocations(const std::vector<DWDDescriptonData> & dwdDescData) {

	m_descData = &dwdDescData;
}

void DWDMap::drawAllDataForYear(unsigned int year){

	double xPos;
	double yPos;

	m_year = year;

	m_size = m_ui->graphicsViewMap->maximumViewportSize();

	m_items[DWDDescriptonData::D_Solar]->setZValue(99);

	QDate minDate (year, 01, 01);
	QDate maxDate (year, 12, 31);


	for ( unsigned int i = 0; i<DWDDescriptonData::DWDDataTypes::NUM_D; ++i) {
		for (QGraphicsItem *item : m_items[i]->childItems() ) {
			m_items[i]->removeFromGroup(item);
			m_scene->removeItem(item);
		}
	}

	for ( unsigned int j = 0; j<m_descData->size(); ++j ) {
		for ( unsigned int i = 0; i<DWDDescriptonData::DWDDataTypes::NUM_D; ++i) {

			const DWDDescriptonData &dwdData = (*m_descData)[j];

			if (!dwdData.m_data[i].m_isAvailable )
				continue;

			if ( !( convertIBKTimeToQDate2(dwdData.m_minDate) < minDate && convertIBKTimeToQDate2(dwdData.m_maxDate) > maxDate ) )
				continue;

			getPosition(m_size, (*m_descData)[j].m_longitude, (*m_descData)[j].m_latitude, xPos, yPos);
			double rad = 5;

			drawLocationInformation(m_items[i], xPos, yPos, m_radius/2, m_opacity, m_color[i]);
		}
	}

	m_scene->update();


	//	drawWeather(m_scene, xPos, yPos, 8, Qt::black);
}

void DWDMap::updateLocationData() {
	for ( unsigned int i = 0; i<DWDDescriptonData::DWDDataTypes::NUM_D; ++i)
		m_items[i]->setOpacity( m_descTypeToDraw[i] ? 1 : 0 );
}

bool DWDMap::getLocation(const std::vector<DWDDescriptonData> & dwdDescData, double &latitude, double &longitude, unsigned int &year, QWidget *parent) {
	DWDMap dwdMap(parent);
	dwdMap.setAllDWDLocations(dwdDescData);
	dwdMap.setLocation(latitude, longitude);
	dwdMap.drawAllDataForYear(year); // we also want to draw all circles with data to the map


	int res = dwdMap.exec();

	if (res = QDialog::Accepted) {
		latitude = dwdMap.m_latitude;
		longitude = dwdMap.m_longitude;
		year = dwdMap.m_year;
		return true; // return invalid object
	}
	else
		return false;
}

void DWDMap::on_checkBoxAirTemp_toggled(bool checked) {
	m_descTypeToDraw[DWDDescriptonData::D_TemperatureAndHumidity] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxRadiation_toggled(bool checked) {
	m_descTypeToDraw[DWDDescriptonData::D_Solar] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxPressure_toggled(bool checked) {
	m_descTypeToDraw[DWDDescriptonData::D_Pressure] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxWind_toggled(bool checked) {
	m_descTypeToDraw[DWDDescriptonData::D_Wind] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxPrecipitation_toggled(bool checked){
	m_descTypeToDraw[DWDDescriptonData::D_Precipitation] = checked;
	updateLocationData();
}

void DWDMap::on_comboBoxYear_currentIndexChanged(const QString &year) {
	drawAllDataForYear(year.toUInt() );
}


void DWDMap::on_horizontalSliderDiameter_valueChanged(int value){
	// radius of all shown data points need to be updated
	m_radius = value;
	drawAllDataForYear(m_year);
}

void DWDMap::on_horizontalSliderOpacity_valueChanged(int value) {
	// radius of all shown data points need to be updated
	m_opacity = value;
	drawAllDataForYear(m_year);
}
