#include "DWDMap.h"
#include "ui_DWDMap.h"
#include "DWDScene.h"

#include "DWDDescriptonData.h"

#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QString>
#include <QDebug>

/*! Converts IBK Time to QDate. */
QDate convertIBKTimeToQDate2(const IBK::Time &time) {

	int y;
	unsigned int m, d;
	double s;

	time.decomposeDate(y, m, d, s);

	return QDate(y, m+1, d+1);

};

void getPosition(QPointF mapPos, int height, int width, const double &longitude, const double &latitude, double &xPos, double &yPos) {

	yPos = mapPos.x() + height - ((latitude - 47.271679) / ( 55.05864 - 47.271679 ))*(height);
	xPos = mapPos.y() + width + ((15.043611 - longitude) / ( 5.866944 - 15.043611 ))*(width);
}

DWDMap::DWDMap(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::DWDMap)
{

	m_ui->setupUi(this);

	int sceneHeight = 900;

	m_scene = new DWDScene(this);

	QGraphicsView *view = m_ui->graphicsViewMap;
	view->setScene(m_scene);

	QSvgRenderer *renderer = new QSvgRenderer();
	renderer->load(QString(":/gfx/Karte_Deutschland.svg"));
	QGraphicsSvgItem *item = new QGraphicsSvgItem();


	item->setSharedRenderer(renderer);

	m_scene->m_mapItem = item;
	m_scene->addItem(item);
	m_scene->setSceneRect(m_scene->sceneRect());


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


	// view->setFixedSize(width, height);
	// view->setSceneRect(0, 0, width, height);
	// view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_color[DT_Location] = Qt::black;
	m_color[DT_Wind] = QColor ("#008b8b");
	m_color[DT_TemperatureAndHumidity] = QColor ("#ffa500");
	m_color[DT_ShortWaveRadiation] = QColor ("#00ff00");
	m_color[DT_Pressure] = QColor ("#0000ff");
	m_color[DT_Precipitation] = QColor ("#ff1493");

	m_ui->checkBoxAirTemp->setStyleSheet("QCheckBox { color: " + m_color[DT_TemperatureAndHumidity].name() + " }");
	m_ui->checkBoxRadiation->setStyleSheet("QCheckBox { color: " + m_color[DT_ShortWaveRadiation].name() + " }");
	m_ui->checkBoxPressure->setStyleSheet("QCheckBox { color: " + m_color[DT_Pressure].name() + " }");
	m_ui->checkBoxWind->setStyleSheet("QCheckBox { color: " + m_color[DT_Wind].name() + " }");
	m_ui->checkBoxPrecipitation->setStyleSheet("QCheckBox { color: " + m_color[DT_Precipitation].name() + " }");

	m_descTypeToDraw[DT_Location] = true;
	m_descTypeToDraw[DT_Wind] = true;
	m_descTypeToDraw[DT_TemperatureAndHumidity] = true;
	m_descTypeToDraw[DT_ShortWaveRadiation] = true;
	m_descTypeToDraw[DT_Pressure] = true;
	m_descTypeToDraw[DT_Precipitation] = true;

	m_items[DT_Location] = new QGraphicsItemGroup();
	m_items[DT_Wind] = new QGraphicsItemGroup();
	m_items[DT_TemperatureAndHumidity] = new QGraphicsItemGroup();
	m_items[DT_ShortWaveRadiation] = new QGraphicsItemGroup();
	m_items[DT_Pressure] = new QGraphicsItemGroup();
	m_items[DT_Precipitation] = new QGraphicsItemGroup();

	m_scene->addItem(m_items[DT_Location]);
	m_scene->addItem(m_items[DT_Wind]);
	m_scene->addItem(m_items[DT_ShortWaveRadiation]);
	m_scene->addItem(m_items[DT_TemperatureAndHumidity]);
	m_scene->addItem(m_items[DT_Pressure]);
	m_scene->addItem(m_items[DT_Precipitation]);
}

DWDMap::~DWDMap() {
	delete m_ui;
}

void DWDMap::mouseMoveEvent(QMouseEvent * event) {
	// we get the position of the mouse

}

void DWDMap::mousePressEvent(QMouseEvent *e) {

	QPointF pos = e->pos();
	QPointF newPos = m_ui->graphicsViewMap->mapToScene(pos.x(), pos.y());
	m_scene->removeItem(m_scene->m_locationItem);
	double thickness = 3;

	QGraphicsEllipseItem *item = new QGraphicsEllipseItem(newPos.x() - m_distance/2, newPos.y() - m_distance/2, m_distance, m_distance);
	item->setPen(QPen(Qt::black, thickness));
	m_scene->addItem(item);
	m_scene->m_locationItem = item;

	// set in lineEditFields
	on_lineEditLatitude_textEdited(QString::number(m_scene->m_latitude) );
	on_lineEditLongitude_textEdited(QString::number(m_scene->m_longitude) );
}

void DWDMap::setLocation(const double &latitude, const double &longitude, const unsigned int &distance) {
	m_latitude = latitude;
	m_longitude = longitude;
	m_distance = distance;

	m_ui->horizontalSliderDistance->setValue(distance);

	double xPos;
	double yPos;

	m_size = m_scene->m_mapItem->boundingRect().size().toSize();

	getPosition(m_scene->m_mapItem->pos(), m_size.height(), m_size.width(), m_longitude, m_latitude, xPos, yPos);

	double rad = m_radius;

	QPointF mapPoint = m_scene->m_mapItem->scenePos();

	qDebug() << "Point Pos: " << xPos << " | " << yPos;
	qDebug() << "Map Pos: " << mapPoint.x() << " | " << mapPoint.y();

	drawDataPoint(DT_Location, m_items[0],  xPos,
											yPos, m_distance, 255, Qt::black);
	m_scene->update();

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

void DWDMap::drawDataPoint(const DataType & type, QGraphicsItemGroup * group, int xPos, int yPos, int rad, int alpha, QColor color) {

	//	scene->addEllipse(xPos-rad, yPos-rad, rad*2.0, rad*2.0,
	//							QPen(color), QBrush(color, Qt::SolidPattern));


	QAbstractGraphicsShapeItem *item;

	switch (type) {
		case DT_Location:
			item = new QGraphicsEllipseItem(xPos-rad/2, yPos-rad/2, rad, rad);
			m_scene->m_locationItem = item;
			item->setPen(QPen (color, 3, Qt::SolidLine));
		break;
		case DT_Precipitation:
		case DT_ShortWaveRadiation:
		case DT_Pressure:
		case DT_Wind:
		case DT_TemperatureAndHumidity:
			item = new QGraphicsEllipseItem(xPos-rad/2, yPos-rad/2, rad, rad);
			item->setPen(QPen (color, 0, Qt::SolidLine));
			color.setAlpha(alpha);
			item->setBrush(QBrush (color) );
		break;
	}
	group->addToGroup(item);

	//	scene->addEllipse(xPos-rad, yPos-rad, rad, rad, QPen(color, 2, Qt::SolidLine), QBrush() );

}

void DWDMap::drawAllDataForYear(unsigned int year){

	double xPos;
	double yPos;

	m_year = year;

	m_size = m_scene->m_mapItem->boundingRect().size().toSize();

	m_items[DWDDescriptonData::D_Solar]->setZValue(1);

	QDate minDate (year, 01, 01);
	QDate maxDate (year, 12, 31);


	for ( unsigned int i = 0; i<DWDDescriptonData::DWDDataType::NUM_D; ++i) {
		// MIND i+1 since Location does not exists in DWDDescriptionData
		for (QGraphicsItem *item : m_items[i+1]->childItems() ) {
			m_items[i+1]->removeFromGroup(item);
			m_scene->removeItem(item);
		}
	}

	for ( unsigned int j = 0; j<m_descData->size(); ++j ) {
		for ( unsigned int i = 0; i<DWDDescriptonData::DWDDataType::NUM_D; ++i) {

			const DWDDescriptonData &dwdData = (*m_descData)[j];

			if (!dwdData.m_data[i].m_isAvailable )
				continue;

			if ( !( convertIBKTimeToQDate2(dwdData.m_minDate) < minDate && convertIBKTimeToQDate2(dwdData.m_maxDate) > maxDate ) )
				continue;

			getPosition(m_scene->m_mapItem->pos(), m_size.height(), m_size.width(), (*m_descData)[j].m_longitude, (*m_descData)[j].m_latitude, xPos, yPos);
			drawDataPoint((DataType)(i+1), m_items[i+1], xPos, yPos, m_radius/2, m_opacity, m_color[i+1]);
		}
	}

	m_scene->update();


	//	drawWeather(m_scene, xPos, yPos, 8, Qt::black);
}

void DWDMap::updateLocationData() {
	for ( unsigned int i = 0; i<DWDDescriptonData::DWDDataType::NUM_D; ++i)
		m_items[i+1]->setOpacity( m_descTypeToDraw[i+1] ? 1 : 0 );
}

bool DWDMap::getLocation(const std::vector<DWDDescriptonData> & dwdDescData, double &latitude, double &longitude, unsigned int &year, unsigned int &distance, QWidget *parent) {
	DWDMap dwdMap(parent);
	dwdMap.setAllDWDLocations(dwdDescData);
	dwdMap.drawAllDataForYear(year); // we also want to draw all circles with data to the map
	dwdMap.setLocation(latitude, longitude, distance);


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
	m_descTypeToDraw[DT_TemperatureAndHumidity] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxRadiation_toggled(bool checked) {
	m_descTypeToDraw[DT_ShortWaveRadiation] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxPressure_toggled(bool checked) {
	m_descTypeToDraw[DT_Pressure] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxWind_toggled(bool checked) {
	m_descTypeToDraw[DT_Wind] = checked;
	updateLocationData();
}

void DWDMap::on_checkBoxPrecipitation_toggled(bool checked){
	m_descTypeToDraw[DT_Precipitation] = checked;
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

void DWDMap::on_lineEditLatitude_textEdited(const QString &lat) {
	bool ok;
	double tempLat = lat.toDouble(&ok);
	if(ok)
		m_latitude = tempLat;

	m_ui->lineEditLatitude->setText(QString("%1").arg(m_latitude));
}


void DWDMap::on_lineEditLongitude_textEdited(const QString &lon) {
	bool ok;
	double tempLat = lon.toDouble(&ok);
	if(ok)
		m_longitude = tempLat;

	m_ui->lineEditLongitude->setText(QString("%1").arg(m_latitude));
}


void DWDMap::on_horizontalSliderDistance_sliderReleased() {
	m_distance = m_ui->horizontalSliderDistance->value();
}

