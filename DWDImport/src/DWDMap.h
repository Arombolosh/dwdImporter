#ifndef DWDMAP_H
#define DWDMAP_H

#include <QDialog>
#include <QGraphicsItem>

class DWDScene;

namespace Ui {
class DWDMap;
}

class DWDMap : public QDialog
{
	Q_OBJECT

public:
	explicit DWDMap(QWidget *parent = nullptr);
	~DWDMap();

	/*! Override Mouse Move Event */
	void mouseMoveEvent(QMouseEvent *event) override;

	/*! Override Mouse Event */
	void mousePressEvent(QMouseEvent *event) override;

	void setLocation(const double &latitude = 0.0, const double &longitude = 0.0);

	static bool getLocation(double &latitude, double &longitude, QWidget *parent = nullptr);

private:
	DWDScene	*m_scene;

	double		m_latitude;
	double		m_longitude;

	Ui::DWDMap	*m_ui;
};

#endif // DWDMAP_H
