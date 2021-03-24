#include "DWDMap.h"
#include "ui_DWDMap.h"
#include "DWDScene.h"

#include <QMouseEvent>


DWDMap::DWDMap(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::DWDMap)
{

	m_ui->setupUi(this);

	m_scene = new DWDScene(this);


	QGraphicsView *view = m_ui->graphicsViewMap;
	view->setScene(m_scene);

	m_scene->addPixmap( QPixmap(":/gfx/Karte_Deutschland.svg") );
	QList<QGraphicsItem*> list = m_scene->items();
	list.at(0)->setZValue(-100);

	qreal width = m_scene->width();
	qreal height = m_scene->height();

	view->setFixedSize(width, height);
	view->setSceneRect(0, 0, width, height);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

DWDMap::~DWDMap() {
	delete m_ui;
}

void DWDMap::mouseMoveEvent(QMouseEvent * event) {
	// we get the position of the mouse

}

void DWDMap::mousePressEvent(QMouseEvent *e) {

	double rad = 5;
	QPointF pt = m_ui->graphicsViewMap->mapToScene(e->pos());

	QList<QGraphicsItem*> list = m_scene->items();
	if ( list.size()>2 )
		m_scene->removeItem( list.at(0) );

	m_scene->addEllipse(pt.x()-rad-18, pt.y()-rad-18, rad*2.0, rad*2.0,
							QPen(), QBrush(Qt::SolidPattern));

	m_latitude = m_scene->m_latitude;
	m_longitude = m_scene->m_longitude;

	// set in lineEditFields
	m_ui->lineEditLatitude->setText( QString::number(m_scene->m_latitude) );
	m_ui->lineEditLongitude->setText( QString::number(m_scene->m_longitude) );
}

void DWDMap::setLocation(const double &latitude, const double &longitude) {
	m_latitude = latitude;
	m_longitude = longitude;

	int xPos;
	int yPos;

	yPos = 796 - ((m_latitude - 47.271679) / ( 55.05864 - 47.271679 ))*796;
	xPos = 588 - ((m_longitude - 15.043611) / ( 5.866944 - 15.043611 ))*588;

	double rad = 5;

	m_scene->addEllipse(xPos-rad-18, yPos-rad-18, rad*2.0, rad*2.0,
							QPen(), QBrush(Qt::SolidPattern));

	m_ui->lineEditLatitude->setText(QString::number(m_latitude));
	m_ui->lineEditLongitude->setText(QString::number(m_longitude));
}

bool DWDMap::getLocation(double &latitude, double &longitude, QWidget *parent) {
	DWDMap dwdMap(parent);
	dwdMap.setLocation(latitude, longitude);

	int res = dwdMap.exec();

	if (res = QDialog::Accepted) {
		latitude = dwdMap.m_latitude;
		longitude = dwdMap.m_longitude;
		return true; // return invalid object
	}
	else
		return false;
}