/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_DoubleWidgetBase.h"

#include <IBK_messages.h>

#include <QHBoxLayout>

#include <QTableView>
#include <QListView>

namespace QtExt {

DoubleWidgetBase::DoubleWidgetBase(QWidget* widget1, QWidget* widget2, QWidget *parent) :
	QWidget(parent),
	m_widget1(widget1),
	m_widget2(widget2)
{

	m_widget1->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
	m_widget1->setParent(parent);
	m_widget2->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
	m_widget2->setParent(parent);

	QHBoxLayout *hBox = new QHBoxLayout;
	hBox->addWidget(m_widget1);
	hBox->addWidget(m_widget2);
	hBox->setMargin(0);
	hBox->setSpacing(0);
	setLayout(hBox);

	// first widget is always visible at first
	m_widget1->setVisible(true);
	m_widget2->setVisible(false);
}

DoubleWidgetBase::~DoubleWidgetBase()
{
}

void DoubleWidgetBase::toggleWidgets() {
	setFirstVisible( !m_widget1->isVisibleTo(this) );
}

void DoubleWidgetBase::setFirstVisible(bool visible) {
	const char * const FUNC_ID = "[DoubleWidgetBase::setFirstVisible]";
	// check if first widget is already visible
	if (m_widget1->isVisibleTo(this) == visible)
		return; // already visible, no need to change anything
	IBK::IBK_Message(IBK::FormatString("Setting first widget %1.\n").arg(visible ? "visible" : "invisible"), IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_DEVELOPER);
	m_widget1->setVisible(visible);
	m_widget2->setVisible(!visible);
	emit widgetsChanged();
	IBK::IBK_Message(IBK::FormatString("Toggling visibility done.\n"), IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_DEVELOPER);
	return;
}

} //widgetsChanged
