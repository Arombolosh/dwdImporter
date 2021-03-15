#ifndef QtExt_LanguageStringEditWidget1H
#define QtExt_LanguageStringEditWidget1H

#include <QWidget>
#include <QDialog>

#include <IBK_MultiLanguageString.h>

namespace QtExt {

class LanguageStringEditWidget3;

namespace Ui {
class LanguageStringEditWidget1;
}

class LanguageStringEditWidget1 : public QWidget
{
	Q_OBJECT

public:
	explicit LanguageStringEditWidget1(QWidget *parent = nullptr);
	~LanguageStringEditWidget1();

	void initLanguages(const std::string& currentLanguage, const std::string& thirdLanguage, bool showLanguageSelection);
	void setString(const IBK::MultiLanguageString& str);

	void set3rdLanguage(const std::string& lang);

	const IBK::MultiLanguageString& string() const;

private slots:
	void on_lineEdit_textChanged(const QString &arg1);

	void on_toolButton_clicked();

private:
	Ui::LanguageStringEditWidget1 * ui;
	IBK::MultiLanguageString		m_string;
	std::string						m_currentLang;
	std::string						m_3rdLang;
	bool							m_showLanguageSelection;
};

class LanguageStringEditDialog3 : public QDialog {
	Q_OBJECT
public:
	explicit LanguageStringEditDialog3(const std::string& currentLanguage, bool showLanguageSelection, QWidget *parent = 0);

	void set(const IBK::MultiLanguageString& str);

	void set3rdLanguage(const std::string& lang);

	const IBK::MultiLanguageString& string() const;

private slots:

private:
	LanguageStringEditWidget3* m_widget;
};

} // namespace QtExt
#endif // QtExt_LanguageStringEditWidget1H
