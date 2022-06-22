#include "DWDProgressBar.h"
#include "ui_DWDProgressBar.h"

#include <QTime>

DWDProgressBar::DWDProgressBar(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::DWDProgressBar)
{
	m_ui->setupUi(this);

	m_ui->progressBar->setMaximum(100);
	m_ui->progressBar->setMaximum(0);
}

DWDProgressBar::~DWDProgressBar() {
	delete m_ui;
}

void DWDProgressBar::startTimer() {
	m_progressTimer.start();
}

void DWDProgressBar::setMaximum(int val) {
	if (m_progressTimer.elapsed() > 100) {
		m_ui->progressBar->setMaximum(val);
		m_progressTimer.start();
	}
}

void DWDProgressBar::setValue(int val) {
	if (m_progressTimer.elapsed() > 100) {
		m_ui->progressBar->setValue(val);
		m_progressTimer.start();
	}
}

void DWDProgressBar::setMinimum(int min) {
	if (m_progressTimer.elapsed() > 100) {
		m_ui->progressBar->setMinimum(min);
		m_progressTimer.start();
	}
}

int DWDProgressBar::value() {
	return m_ui->progressBar->value();
}

void DWDProgressBar::setTitle(QString title) {
	setWindowTitle(title);
}

void DWDProgressBar::addText(QString text) {
	text = QString("[%1] %2").arg(QTime::currentTime().toString())
							.arg(text);

	m_ui->textBrowserProgress->append(text);
}
