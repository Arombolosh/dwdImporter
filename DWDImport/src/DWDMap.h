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

private:
	Ui::DWDMap *ui;
};

#endif // DWDMAP_H
