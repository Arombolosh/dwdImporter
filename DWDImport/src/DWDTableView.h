#ifndef DWDTableViewH
#define DWDTableViewH

#include <QWidget>
#include <QTableView>

#include "DWDDescriptonData.h"

namespace Ui {
class DWDTableView;
}

class DWDTableView : public QWidget
{
	Q_OBJECT

public:
	explicit DWDTableView(QWidget *parent = nullptr);
	~DWDTableView();

	/*! Function to update the table contents */
	void updateTableUI();

private:
	Ui::DWDTableView									*ui;

	/*! Pointer to database */
	std::map<unsigned int, DWDDescriptonData>			*m_descDataMap;
};

#endif // DWDTableViewH
