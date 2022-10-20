#ifndef DM_MapWidgetH
#define DM_MapWidgetH

#include <QWidget>

namespace Ui {
class MapWidget;
}

namespace DM {
class MapWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MapWidget(QWidget *parent = nullptr);
	~MapWidget();

private:
	Ui::MapWidget *m_ui;
};
}

#endif // DM_MapWidgetH
