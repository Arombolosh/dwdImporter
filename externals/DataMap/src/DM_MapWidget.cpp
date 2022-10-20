#include "DM_MapWidget.h"
#include "ui_DM_MapWidget.h"

namespace DM {

MapWidget::MapWidget(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::MapWidget)
{
	m_ui->setupUi(this);
}

MapWidget::~MapWidget()
{
	delete m_ui;
}

}
