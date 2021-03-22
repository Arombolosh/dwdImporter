#ifndef DWDMAP_H
#define DWDMAP_H

#include <QDialog>

namespace Ui {
class DWDMap;
}

class DWDMap : public QDialog
{
	Q_OBJECT

public:
	explicit DWDMap(QWidget *parent = nullptr);
	~DWDMap();

	/*! Override Mouse Event */
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	Ui::DWDMap *m_ui;
};

#endif // DWDMAP_H
