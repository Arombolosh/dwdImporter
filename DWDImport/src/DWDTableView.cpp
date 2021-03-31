#include "DWDTableView.h"
#include "ui_DWDTableView.h"

DWDTableView::DWDTableView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DWDTableView)
{
	ui->setupUi(this);

}

DWDTableView::~DWDTableView() {
	delete ui;
}

void DWDTableView::updateTableUI() {


}
