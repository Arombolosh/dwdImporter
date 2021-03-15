#include "QtExt_ComboBoxCountries.h"

#include <QFile>
#include <QTextStream>

namespace QtExt {

ComboBoxCountries::ComboBoxCountries(QWidget *parent) :
	QComboBox (parent)
{

	 QFile file(":/text/countries.txt");
	 file.open(QIODevice::ReadOnly);
	 QTextStream txtStrm(&file);
	 QStringList list;
	 QString str;
	 do {
		str = txtStrm.readLine();
		if(!str.isEmpty())
			list << str;
	 } while(str != QString());

	 addItem("", tr("None"));
	 foreach (const QString& str, list) {
		 QStringList tl = str.split('|');
		 Q_ASSERT(tl.size() == 2);
		 addItem(tl[1], tl[0]);
	 }
	 setEditable(false);
}

QString ComboBoxCountries::countryName() const {
	return currentText();
}

QString ComboBoxCountries::countryShort() const {
	return currentData().toString();
}

void ComboBoxCountries::setCountry(const QString& country) {
	int index = findText(country);
	if(index == -1) {
		addItem(country);
		setCurrentIndex(count() - 1);
	}
	else {
		setCurrentIndex(index);
	}
}

} // namespace QtExt
