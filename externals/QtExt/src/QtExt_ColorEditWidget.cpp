/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QColorDialog>

#include "QtExt_ColorEditWidget.h"

namespace QtExt {


ColorEditWidget::ColorEditWidget(QWidget * parent) :
	QWidget(parent)
{
	setupUi();
}

ColorEditWidget::ColorEditWidget(const QString & label, QWidget * parent) :
	QWidget(parent)
{
	setupUi();
	m_label->setText(label);
}

void ColorEditWidget::setupUi() {
	m_label = new QLabel(this);

	m_colorWidget = new QFrame(this);
	m_colorWidget->setFrameShape(QFrame::Box);
	m_colorWidget->setMinimumSize(48,24);
	m_colorWidget->setMaximumSize(48,24);
	m_colorWidget->setAutoFillBackground(true);

	m_pushButton = new QPushButton(this);
	m_pushButton->setText(tr("..."));
	m_pushButton->setMaximumWidth(48);

	connect(m_pushButton, SIGNAL(clicked()), this, SLOT(onSelectColor()));

	setMaximumHeight(m_pushButton->height());
	QHBoxLayout * lay = new QHBoxLayout;
	lay->addWidget(m_label);
	lay->addWidget(m_colorWidget);
	lay->addWidget(m_pushButton);
	lay->setContentsMargins(0,0,0,0);
	setLayout(lay);
}

void ColorEditWidget::setLabel(const QString & label) {
	m_label->setText(label);
}

void ColorEditWidget::setColor(const QColor & color) {
	QPalette pal = m_colorWidget->palette();
	pal.setColor(QPalette::Background, color);
	m_colorWidget->setPalette(pal);
}

QColor ColorEditWidget::color() const {
	return m_colorWidget->palette().color(QPalette::Background);
}

void ColorEditWidget::onSelectColor() {
	QColorDialog dlg(color(), this);
	if (dlg.exec() == QDialog::Accepted) {
		setColor(dlg.selectedColor());
		emit colorSelected(dlg.selectedColor());
	}
}

} // namespace QtExt
