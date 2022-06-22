#ifndef DWDPROGRESSBAR_H
#define DWDPROGRESSBAR_H

#include <QDialog>
#include <QElapsedTimer>

namespace Ui {
class DWDProgressBar;
}

class DWDProgressBar : public QDialog
{
	Q_OBJECT

public:
	explicit DWDProgressBar(QWidget *parent = nullptr);
	~DWDProgressBar();

	void startTimer();

	void setMaximum(int max);
	void setMinimum(int min);
	void setValue(int val);

	int value();

	void setTitle(QString title);
	void addText(QString text);

private:
	Ui::DWDProgressBar *m_ui;


	QElapsedTimer		m_progressTimer;
};

#endif // DWDPROGRESSBAR_H
