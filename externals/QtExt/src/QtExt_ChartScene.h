/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_ChartSceneH
#define QtExt_ChartSceneH

#include <QObject>
#include <QGraphicsScene>

namespace QtExt {

class ChartScene : public QGraphicsScene {
	Q_OBJECT
public:
	ChartScene(bool onScreen, QPaintDevice *device, QObject* parent = 0);

	/*! The main interface for painting the diagram.*/
	void setup(QRect frame, QPaintDevice *device, double res);

protected:
	qreal penWidth(qreal minWidth, qreal resWidth);

	double m_topBorder;
	double m_bottomBorder;
	double m_leftBorder;
	double m_rightBorder;

	/*! Line width of the frame rectangle.*/
	double				m_frameWidth;

	/*! Scene rectangle.*/
	QRectF				m_frame;

	/*! Chart rectangle.*/
	QRectF				m_chartFrame;

	/*! True if chart is drawn on screen, false if chart is printed
		or exported as picture.
	*/
	bool				m_onScreen;

	/*! The resolution of the chart on a printer or exported image in pixel/mm.
		This is used to calculate the various distances and lengths.
	*/
	double				m_res;
	QPaintDevice*		m_device;					///< Paintdevice.

};

} // end namespace QtExt

#endif // QtExt_ChartSceneH
