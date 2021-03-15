/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include <QPainter>
#include <QDebug>
#include <QResizeEvent>

#include "QtExt_TableWidget.h"
#include "QtExt_Table.h"

namespace QtExt {

TableWidget::TableWidget(QtExt::Table* table, QWidget *parent) : QWidget(parent),
		m_table(table)
{
	m_table->setTableSize(geometry().size());
	connect(m_table, SIGNAL(changed()), this, SLOT(repaintTable()));
}

Table* TableWidget::table() {
	return m_table;
}

void TableWidget::paintEvent ( QPaintEvent * event ) {
	// paint parent class and initialize viewport
	QWidget::paintEvent(event);
	QPainter painter(this);
	m_table->drawTable(&painter, QPointF(0, 0));
}

void TableWidget::resizeEvent ( QResizeEvent * event ) {
	QWidget::resizeEvent(event);
	m_table->setTableSize(event->size());
}

void TableWidget::repaintTable() {
	repaint(geometry());
}

} // namespace QtExt
