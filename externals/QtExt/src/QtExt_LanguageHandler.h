/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_LanguageHandlerH
#define QtExt_LanguageHandlerH

#include <QString>
#include <QCoreApplication>

class QTranslator;

namespace QtExt {

/*! Central class that handles language switching.
	\code
	// example usage

	// first configure language handler with application settings
	LanguageHandler::setup("IBK", "PostProc 2.0", "PostProc2", translationPath);

	// install german translation
	LanguageHandler::instance()->installTranslator("de");

	// loads language file 'PostProc2_de.qm'
	\endcode
*/
class LanguageHandler {
	Q_DISABLE_COPY(LanguageHandler)
public:
	/*! Returns an instance of the language handler singleton. */
	static LanguageHandler & instance();

	/*! Destructor, removes language handler objects. */
	~LanguageHandler();

	/*! Initializes the language handler with application-specific constants. */
	static void setup(const QString & organization, const QString & program,
					  const QString & translationDir, const QString & languageFilePrefix);

	/*! Returns current language ID. */
	static QString langId();

	/*! Sets the language Id in the settings object. */
	static void setLangId(QString id);

	/*! Installs the translator identified by langId and stores the
		language ID in the program settings. */
	void installTranslator(QString langId);

private:
	static QString		m_organization;
	static QString		m_program;
	static QString		m_translationDir;
	static QString		m_languageFilePrefix;

	/*! The translater for the strings of the program itself. */
	QTranslator * applicationTranslator;
	/*! The translater for strings of the standard dialogs and other Qt library
		messages. */
	QTranslator * systemTranslator;

	/*! Upon construction the translator objects are created and
		the last used translation setting is installed. */
	LanguageHandler();

};

} // namespace QtExt


#endif // QtExt_LanguageHandlerH
