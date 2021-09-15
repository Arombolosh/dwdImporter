#include "DWDSortFilterProxyModel.h"


DWDSortFilterProxyModel::DWDSortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
{
}

void DWDSortFilterProxyModel::setFilterMinimumDate(QDate date) {
	m_minDate = date;
	invalidateFilter();
}

void DWDSortFilterProxyModel::setFilterMaximumDate(QDate date) {
	m_maxDate = date;
	invalidateFilter();
}

bool DWDSortFilterProxyModel::dateInRange(QDate date) const {
	return (!m_minDate.isValid() || date > m_minDate) && (!m_maxDate.isValid() || date < m_maxDate);
}
