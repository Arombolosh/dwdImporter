#include "DWDProgressBar.h"
#include "ui_DWDProgressBar.h"

DWDProgressBar::DWDProgressBar(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DWDProgressBar)
{
	ui->setupUi(this);
}

DWDProgressBar::~DWDProgressBar()
{
	delete ui;
}
