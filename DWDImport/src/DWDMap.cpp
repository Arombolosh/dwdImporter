#include "DWDMap.h"
#include "ui_DWDMap.h"

DWDMap::DWDMap(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DWDMap)
{
	ui->setupUi(this);
}

DWDMap::~DWDMap()
{
	delete ui;
}
