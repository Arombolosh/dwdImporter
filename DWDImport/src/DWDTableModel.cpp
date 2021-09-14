#include "DWDTableModel.h"


DWDTableModel::DWDTableModel(QObject * parent):
	QAbstractTableModel(parent)
{

}

int DWDTableModel::rowCount(const QModelIndex & parent) const {
	return m_descDataMap->size();
}

int DWDTableModel::columnCount(const QModelIndex & parent) const {
	return 10;
}

QVariant DWDTableModel::data(const QModelIndex & index, int role) const {

	std::map<unsigned int, DWDDescriptonData>::iterator itDWDData = m_descDataMap->begin();

//	for (int i = 0; i<index.row(); ++i)
//		++itDWDData;

	std::advance(itDWDData, index.row() );

	DWDDescriptonData & dwdData = itDWDData->second;

	switch (role) {
		case Qt::DisplayRole :
			switch (index.column()) {
				case ColId :
					return dwdData.m_idStation;
				case ColDistance :
					return dwdData.m_distance;
				case ColWind :
					return dwdData.m_data[DWDDescriptonData::D_Wind];
				case ColAirTemp:
					return dwdData.m_data[DWDDescriptonData::D_Wind];
				case ColPressure :
					return dwdData.m_data[DWDDescriptonData::D_Pressure];
				case ColRadiation :
					return dwdData.m_data[DWDDescriptonData::D_Solar];
				case ColCountry :
					return QString::fromStdString(dwdData.m_country);
				case ColLatitude :
					return dwdData.m_latitude;
				case ColLongitude :
					return dwdData.m_longitude;
				case ColName :
					return QString::fromStdString(dwdData.m_name);

		}
		break;

		case Qt::FontRole :
			// vars with INVALID valueRef -> grey italic
			//      with valid value -> black, bold
		break;

		case Qt::ForegroundRole :
			// vars with INVALID valueRef -> grey italic
		break;

		// UserRole returns value reference
		case Qt::UserRole :
//			return (*m_availableVariables)[(size_t)index.row()].m_fmiValueRef;
		break;
	}
	return QVariant();

}


QVariant DWDTableModel::headerData(int section, Qt::Orientation orientation, int role) const {

		static QStringList headers = QStringList()

			<< tr("Station Id")
			<< tr("Distance")
			<< tr("Location Longitude Deg")
			<< tr("Location Latitude Deg")
			<< tr("Name")
			<< tr("Country")
			<< tr("T_air + rH")
			<< tr("Radiation")
			<< tr("Wind")
			<< tr("Pressure");

		if (orientation == Qt::Vertical)
			return QVariant();
		switch (role) {
			case Qt::DisplayRole :
				return headers[section];
		}
		return QVariant();
}
