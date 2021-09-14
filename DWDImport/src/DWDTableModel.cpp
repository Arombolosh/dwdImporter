#include "DWDTableModel.h"

DWDTableModel::DWDTableModel()
{

}

int DWDTableModel::rowCount(const QModelIndex & parent) const {
	return m_descDataMap->size();
}

int DWDTableModel::columnCount(const QModelIndex & parent) const {
	return 10;
}

QVariant DWDTableModel::data(const QModelIndex & index, int role) const {
	DWDDescriptonData & dwdData = m_descDataMap[(size_t)index.row()];

	switch (role) {
		case Qt::DisplayRole :
			switch (index.column()) {
				case 0 : // station ID
					return dwdData.m_id;
				case 1 : {// distance
//					return dwdData->m_;
				}

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
	}
	return QVariant();

}

bool DWDTableModel::setData(const QModelIndex & index, const QVariant & value, int role) {

}
