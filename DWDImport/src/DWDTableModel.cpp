#include "DWDTableModel.h"


QDate convertIBKTimeToQDate(const IBK::Time &time) {

	int y;
	unsigned int m, d;
	double s;

	time.decomposeDate(y, m, d, s);

	return QDate(y, m+1, d+1);

};

DWDTableModel::DWDTableModel(QObject * parent):
	QAbstractTableModel(parent)
{

}

int DWDTableModel::rowCount(const QModelIndex & parent) const {
	return m_descData->size();
}

int DWDTableModel::columnCount(const QModelIndex & parent) const {
	return 12;
}

QVariant DWDTableModel::data(const QModelIndex & index, int role) const {

	if (!index.isValid()) // we do not want to take an invalid index
		return QVariant();

	DWDDescriptonData & dwdData = (*m_descData)[index.row()];

	DWDDescriptonData::DWDDataTypes dataType = DWDDescriptonData::NUM_D;

	switch (index.column() ) {
		case ColWind:
			dataType = DWDDescriptonData::D_Wind;
		break;
		case ColAirTemp:
			dataType = DWDDescriptonData::D_TemperatureAndHumidity;
		break;
		case ColPressure:
			dataType = DWDDescriptonData::D_Pressure;
		break;
		case ColRadiation:
			dataType = DWDDescriptonData::D_Solar;
		break;
	}

	switch (role) {
		case Qt::DisplayRole :
			switch (index.column()) {
				case ColId :
					return dwdData.m_idStation;
				case ColDistance :
					return dwdData.m_distance;
				case ColCountry :
					return QString::fromLatin1(dwdData.m_country.c_str());
				case ColLatitude :
					return dwdData.m_latitude;
				case ColLongitude :
					return dwdData.m_longitude;
				case ColName :
					return QString::fromLatin1(dwdData.m_name.c_str());
				case ColMinDate :
					return convertIBKTimeToQDate(dwdData.m_minDate);
				case ColMaxDate :
					return convertIBKTimeToQDate(dwdData.m_maxDate);
		}
		break;

		case Qt::CheckStateRole : // we also want to apply all the checkboxes
			switch (index.column()) {
				case ColWind :
				case ColAirTemp :
				case ColRadiation :
				case ColPressure :
					Q_ASSERT(dataType != DWDDescriptonData::DWDDataTypes::NUM_D);
					if (!dwdData.m_data[dataType].m_isAvailable)
						return QVariant();
					else
						return dwdData.m_data[dataType].m_isChecked ? Qt::Checked : Qt::Unchecked;
			}
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
			<< tr("Longitude")
			<< tr("Latitude")
			<< tr("Name")
			<< tr("Country")
			<< tr("Minimum Date")
			<< tr("Maximum Date")
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

bool DWDTableModel::setData(const QModelIndex & index, const QVariant & value, int role) {

	if (!index.isValid())
		return false;

	int col = index.column();
	DWDDescriptonData::DWDDataTypes dataType;

	switch (col) {
		case ColWind:
			dataType = DWDDescriptonData::D_Wind;
		break;
		case ColAirTemp:
			dataType = DWDDescriptonData::D_TemperatureAndHumidity;
		break;
		case ColPressure:
			dataType = DWDDescriptonData::D_Pressure;
		break;
		case ColRadiation:
			dataType = DWDDescriptonData::D_Solar;
		break;
	}

	DWDDescriptonData &checkBox = (*m_descData)[index.row()];

	QModelIndex uncheckedIndex;

	if (role == Qt::CheckStateRole)	{
		if ((Qt::CheckState)value.toInt() == Qt::Checked) {
			//	user has checked item
			//  we have to uncheck all the data in the column if we check a different value
			for ( DWDDescriptonData &dwdData : (*m_descData) ) {
				if (dwdData.m_data[dataType].m_isChecked) { // we find where we need to update our view
					dwdData.m_data[dataType].m_isChecked = false; // we uncheck the checkbox
				}

				// get index range
				QModelIndex top = QAbstractTableModel::index(0,index.column() );
				QModelIndex bottom = QAbstractTableModel::index((*m_descData).size(), index.column() );
				emit dataChanged(top, bottom); // we just update the whole column
			}

			checkBox.m_data[dataType].m_isChecked = true;
		}
		else {
			checkBox.m_data[dataType].m_isChecked = false; //user has unchecked item

			emit dataChanged(index, index);
		}

		return true;
	}
	return false;

}

Qt::ItemFlags DWDTableModel::flags(const QModelIndex & index) const {

	if (!index.isValid()) // assert that index is valid
		return Qt::ItemFlags();

	int col = index.column();
	DWDDescriptonData &checkBox = (*m_descData)[index.row()];

	switch (col) {
		case 6:
			return checkBox.m_data[DWDDescriptonData::D_TemperatureAndHumidity].m_isAvailable ? QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable : QAbstractItemModel::flags(index) ;
		case 7:
			return checkBox.m_data[DWDDescriptonData::D_Solar].m_isAvailable ? QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable : QAbstractItemModel::flags(index) ;
		case 8:
			return checkBox.m_data[DWDDescriptonData::D_Wind].m_isAvailable ? QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable : QAbstractItemModel::flags(index) ;
		case 9:
			return checkBox.m_data[DWDDescriptonData::D_Pressure].m_isAvailable ? QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable : QAbstractItemModel::flags(index) ;
	}


	return QAbstractItemModel::flags(index);

}
