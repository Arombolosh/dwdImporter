#ifndef DWDTableModelH
#define DWDTableModelH

#include <QObject>
#include <QAbstractTableModel>

#include "DWDDescriptonData.h"

class DWDTableModel : public QAbstractTableModel {
	Q_OBJECT
public:
	DWDTableModel(QObject *parent);

	/*! Columns shown in the table view. */
	enum Columns {
		ColId,
		ColDistance,
		ColLongitude,
		ColLatitude,
		ColName,
		ColCountry,
		ColAirTemp,
		ColRadiation,
		ColWind,
		ColPressure,
		NumColumns
	};

	// QAbstractItemModel interface
public:
	int rowCount(const QModelIndex & parent) const;
	int columnCount(const QModelIndex & parent) const;
	QVariant data(const QModelIndex & index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;



	/*! Description input of all stations.
		Map Cointainging all necessairy data about the weather station
		key of map is station id
	*/
	std::map<unsigned int, DWDDescriptonData>				*m_descDataMap = nullptr;

	// QAbstractItemModel interface
public:
};

#endif // DWDTableModelH
