#ifndef DWDMAP_H
#define DWDMAP_H

#include <QDialog>
#include <QGraphicsItem>
#include <QColor>
#include <QGraphicsItemGroup>

#include "DWDDescriptonData.h"

class DWDScene;


namespace Ui {
class DWDMap;
}

class DWDMap : public QDialog
{
	Q_OBJECT

	enum DataType {
		DT_Precipitation,
		DT_TemperatureAndHumidity,
		DT_ShortWaveRadiation,
		DT_Wind,
		DT_Pressure
	};

public:
	explicit DWDMap(QWidget *parent = nullptr);
	~DWDMap();

	/*! Override Mouse Move Event */
	void mouseMoveEvent(QMouseEvent *event) override;

	/*! Override Mouse Event */
	void mousePressEvent(QMouseEvent *event) override;

	/*! Sets the currently picked location */
	void setLocation(const double &latitude = 0.0, const double &longitude = 0.0);

	/*! Sets the year needed for data extraction */
	void setYear(const unsigned int &year = 2020);

	/*! Add all the locations */
	void setAllDWDLocations(const std::vector<DWDDescriptonData> & dwdDescData);

	/*! Draws a Location specific data point*/
	void drawDataPoint(const DWDDescriptonData::DWDDataType & type, QGraphicsItemGroup *group, int xPos, int yPos, int rad, int alpha, QColor color);

	/*! Draw all data */
	void drawAllDataForYear(unsigned int year);

	/*! Updates the Map with all weather data that is checked (checkboxes in UI) */
	void updateLocationData();

	/*! Static call to get a location from the map */
	static bool getLocation(const std::vector<DWDDescriptonData> & dwdDescData, double &latitude, double &longitude, unsigned int & year, QWidget *parent = nullptr);

private slots:
	void on_checkBoxAirTemp_toggled(bool checked);

	void on_checkBoxRadiation_toggled(bool checked);

	void on_checkBoxPressure_toggled(bool checked);

	void on_checkBoxWind_toggled(bool checked);

	void on_comboBoxYear_currentIndexChanged(const QString &arg1);

	void on_checkBoxPrecipitation_toggled(bool checked);

	void on_horizontalSliderDiameter_valueChanged(int value);

	void on_horizontalSliderOpacity_valueChanged(int value);

private:
	DWDScene								*m_scene;

	QSize									m_size;

	double									m_latitude;
	double									m_longitude;

	/*! Year for wich data is shown. */
	unsigned int							m_year;

	/*! Radius of data points in scene.*/
	unsigned int							m_radius = 15;
	unsigned int							m_opacity = 255;

	bool									m_descTypeToDraw[DWDDescriptonData::DWDDataType::NUM_D];
	QColor									m_color[DWDDescriptonData::DWDDataType::NUM_D];
	QGraphicsItemGroup						*m_items[DWDDescriptonData::DWDDataType::NUM_D] = {nullptr, nullptr, nullptr, nullptr, nullptr};

	const std::vector<DWDDescriptonData>	*m_descData = nullptr;

	Ui::DWDMap								*m_ui;
};

#endif // DWDMAP_H
