#include "DM_GraphicsView.h"

#include <QWheelEvent>

namespace DM {
GraphicsView::GraphicsView(QWidget *parent) :
	QGraphicsView(parent)
{
	setRenderHint(QPainter::Antialiasing);
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::wheelEvent(QWheelEvent * event) {
//	if (event->modifiers() & Qt::ControlModifier) {
//		// zoom
//		const ViewportAnchor anchor = transformationAnchor();
//		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//		int angle = event->angleDelta().y();
//		qreal factor;
//		if (angle > 0) {
//			factor = 1.1;
//		} else {
//			factor = 0.9;
//		}
//		scale(factor, factor);
//		setTransformationAnchor(anchor);
//	} else {
//		QGraphicsView::wheelEvent(event);
//	}
}

void GraphicsView::resizeEvent(QResizeEvent * event) {
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
	setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}
}
