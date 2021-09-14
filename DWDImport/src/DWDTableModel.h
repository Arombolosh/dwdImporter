#ifndef DWDTableModelH
#define DWDTableModelH

#include <QObject>
#include <QAbstractTableModel>

#include "DWDDescriptonData.h"

class DWDTableModel : public QAbstractTableModel {
	Q_OBJECT
public:
	DWDTableModel();



	// QAbstractItemModel interface
public:
	int rowCount(const QModelIndex & parent) const;
	int columnCount(const QModelIndex & parent) const;
	QVariant data(const QModelIndex & index, int role) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role);



	/*! Description input of all stations.
		Map Cointainging all necessairy data about the weather station
		key of map is station id
	*/
	std::map<unsigned int, DWDDescriptonData>				*m_descDataMap = nullptr;
};

#endif // DWDTableModelH
