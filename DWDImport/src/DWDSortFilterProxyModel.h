#ifndef DWDSortFilterProxyModelH
#define DWDSortFilterProxyModelH

#include <QObject>
#include <QSortFilterProxyModel>
#include <QDate>

class DWDSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	DWDSortFilterProxyModel(QObject *parent = nullptr);

	QDate filterMinimumDate() const { return m_minDate; }
	void setFilterMinimumDate(QDate date);

	QDate filterMaximumDate() const { return m_minDate; }
	void setFilterMaximumDate(QDate date);

protected:

private:
	bool dateInRange(QDate date) const;

	QDate				m_minDate;
	QDate				m_maxDate;
};

#endif // DWDSortFilterProxyModelH
