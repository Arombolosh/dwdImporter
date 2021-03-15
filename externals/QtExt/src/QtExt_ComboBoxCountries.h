#ifndef QtExt_ComboBoxCountriesH
#define QtExt_ComboBoxCountriesH

#include <QObject>
#include <QComboBox>

namespace QtExt {

class ComboBoxCountries : public QComboBox {
	Q_OBJECT
public:
	explicit ComboBoxCountries(QWidget *parent = 0);

	QString countryName() const;

	QString countryShort() const;

	void setCountry(const QString& country);

};

} // namespace QtExt

#endif // QtExt_ComboBoxCountriesH
