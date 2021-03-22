#include "DWDMap.h"
#include "ui_DWDMap.h"

#include <QMouseEvent>

DWDMap::DWDMap(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::DWDMap) {

	m_ui->setupUi(this);

	m_ui->labelPixmap;
}

DWDMap::~DWDMap() {
	delete m_ui;
}

void DWDMap::mouseMoveEvent(QMouseEvent * event) {
	// we get the position of the mouse


}
