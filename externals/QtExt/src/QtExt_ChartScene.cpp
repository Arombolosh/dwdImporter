/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_ChartScene.h"

namespace QtExt {

ChartScene::ChartScene(bool onScreen, QPaintDevice *device, QObject* parent) :
	QGraphicsScene(parent),
	m_topBorder(0.05),
	m_bottomBorder(0.1),
	m_leftBorder(0.1),
	m_rightBorder(0.05),
	m_frameWidth(1),
	m_onScreen(onScreen),
	m_res(1.0),
	m_device(device)
{

}

qreal ChartScene::penWidth(qreal minWidth, qreal resWidth) {
	return m_res <= 1 ? minWidth : std::max(resWidth * m_res, minWidth);
}

void ChartScene::setup(QRect frame, QPaintDevice *device, double res) {
	Q_ASSERT(device);
	m_device = device;
	m_res = res;
	// If no change no calculations needed
	bool noCalculationNeeded = frame == m_frame;

	if( noCalculationNeeded )
		return;

	m_frame = frame;
}

} // end namespace QtExt
