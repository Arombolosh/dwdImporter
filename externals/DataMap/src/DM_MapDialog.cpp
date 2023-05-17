#include "DM_MapDialog.h"
#include "ui_DM_MapDialog.h"

#include "DM_Scene.h"


namespace DM {

MapDialog::MapDialog(QWidget *parent) :
    QDialog(parent),
	m_ui(new Ui::MapDialog)
{
	m_ui->setupUi(this);

	// init Scene
	m_scene = new Scene(this);
	m_ui->graphicsViewMap->setScene(m_scene);
	m_scene->setSceneRect(m_scene->sceneRect());

	// fit scene in view
	m_ui->graphicsViewMap->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);

	// draw al data points
//	for(const Data &d : m_data)
//		m_scene->addDwdDataPoint(d.m_type, d.m_name, d.m_minDate, d.m_maxDate, d.m_latitude, d.m_longitude);
}

MapDialog::~MapDialog() {
	delete m_ui;
}


void MapDialog::on_checkBoxAirTemp_toggled(bool checked) {
	m_scene->setItemGroupVisiblity(Data::DT_TemperatureAndHumidity, checked);
}


void MapDialog::on_checkBoxRadiation_toggled(bool checked) {
	m_scene->setItemGroupVisiblity(Data::DT_Solar, checked);
}


void MapDialog::on_checkBoxPressure_toggled(bool checked) {
	m_scene->setItemGroupVisiblity(Data::DT_Pressure, checked);
}


void MapDialog::on_checkBoxWind_toggled(bool checked) {
	m_scene->setItemGroupVisiblity(Data::DT_Wind, checked);
}


void MapDialog::on_checkBoxPrecipitation_toggled(bool checked) {
	m_scene->setItemGroupVisiblity(Data::DT_Precipitation, checked);
}



} // namespace DM
