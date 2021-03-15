/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_BrowseFilenameWidget.h"

#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QFileInfo>

#include <IBK_Path.h>

namespace QtExt {

BrowseFilenameWidget::BrowseFilenameWidget(QWidget *parent) :
	QWidget(parent),
	m_filenameMode(true),
	m_fileMustExist(true)
{
	m_lineEdit = new QLineEdit(this);
	m_toolBtn = new QToolButton(this);
	m_toolBtn->setText("...");
	QHBoxLayout * lay = new QHBoxLayout;
	lay->addWidget(m_lineEdit);
	lay->addWidget(m_toolBtn);
	setLayout(lay);
	lay->setMargin(0);

	connect(m_toolBtn, SIGNAL(clicked()), this, SLOT(onToolBtnClicked()));
	connect(m_lineEdit, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
	connect(m_lineEdit, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
}


void BrowseFilenameWidget::setup(const QString &filename, bool filenameMode, bool fileMustExist, const QString & filter) {
	m_filenameMode = filenameMode;
	m_fileMustExist = fileMustExist;
	m_filter = filter;
	m_lineEdit->setText(filename);
}


void BrowseFilenameWidget::setReadOnly(bool readOnly) {
	m_toolBtn->setEnabled(!readOnly);
	m_lineEdit->setReadOnly(readOnly);
}


void BrowseFilenameWidget::setFilename(const QString & filename) {
	// replace potentially existing relative path paths
	// but that does not mean we always want an absolute path!!!
	IBK::Path f(filename.toStdString());
	f.removeRelativeParts();
	m_lineEdit->setText(QString::fromStdString(f.str()));
}

QString BrowseFilenameWidget::filename() const {
	return m_lineEdit->text().trimmed();
}


void BrowseFilenameWidget::onToolBtnClicked() {
	QString fn;
	blockSignals(true);
	if (m_filenameMode) {
		if (m_fileMustExist) {
			fn = QFileDialog::getOpenFileName(nullptr, tr("Select filename"), filename(), m_filter);
		}
		else {
			fn = QFileDialog::getSaveFileName(this, tr("Select filename"), filename(), m_filter);
		}
	}
	else {
		if (m_fileMustExist) {
			fn = QFileDialog::getExistingDirectory(this, tr("Select filename"), filename());
		}
		else {
			fn = QFileDialog::getSaveFileName(this, tr("Select directory"), filename(), m_filter);
		}
	}
	blockSignals(false);
	if (!fn.isEmpty()) {
		m_lineEdit->setText(fn);
		emit editingFinished();
	}
}

} // namespace QtExt
