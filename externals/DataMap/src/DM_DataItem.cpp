#include "DM_DataItem.h"

#include <QPen>
#include <QPainter>

namespace DM {

const Data &DataItem::data() const {
	return m_data;
}

QRectF DataItem::boundingRect() const {
	// Return the bounding rect for your item.
	// In this case, it is a m_sizexm_size square.
	QRectF rect(m_position.x() - m_size/2 - m_width,
					  m_position.y() - m_size/2 - m_width,
					  m_size + 2*m_width,
				m_size + 2*m_width);

	return rect;
}

void DataItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	if (m_data.m_minDate.secondsUntil(*m_minDate) < 0)
		return;


	if (m_data.m_maxDate.secondsUntil(*m_maxDate) > 0)
		return;

	// Draw two lines
	QPen pen(m_color);
	pen.setWidth(m_width);

//	painter->setPen(pen);
	painter->setPen(Qt::NoPen);
	painter->setBrush(m_color);

	setOpacity(1.0);
	if(m_data.m_currentDistance > *m_maximumDistance)
		setOpacity(0.2);

	Q_ASSERT(m_maximumDistance != nullptr);

	// Adjust these values to change the lines
//	painter->drawLine(m_position.x() - m_size/2, m_position.y() - m_size/2,
//					  m_position.x() + m_size/2, m_position.y() + m_size/2);
//	painter->drawLine(m_position.x() - m_size/2, m_position.y() + m_size/2,
//					  m_position.x() + m_size/2, m_position.y() - m_size/2);

	painter->drawEllipse(m_position.x() - m_size/2, m_position.y() - m_size/2,
						 m_size, m_size);

	setZValue(50); // always on top
}

}

