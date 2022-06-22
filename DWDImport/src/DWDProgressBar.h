#ifndef DWDPROGRESSBAR_H
#define DWDPROGRESSBAR_H

#include <QWidget>

namespace Ui {
class DWDProgressBar;
}

class DWDProgressBar : public QWidget
{
	Q_OBJECT

public:
	explicit DWDProgressBar(QWidget *parent = nullptr);
	~DWDProgressBar();

private:
	Ui::DWDProgressBar *ui;
};

#endif // DWDPROGRESSBAR_H
