/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_LanguageHandler.h"
#include <QTranslator>
#include <QSettings>
#include <QCoreApplication>
#include <QLocale>
#include <QDebug>

#include <IBK_messages.h>
#include <IBK_FormatString.h>

namespace QtExt {

QString		LanguageHandler::m_organization;
QString		LanguageHandler::m_program;
QString		LanguageHandler::m_translationDir;
QString		LanguageHandler::m_languageFilePrefix;

LanguageHandler & LanguageHandler::instance() {
	static LanguageHandler myHandler;
	return myHandler;
}


LanguageHandler::LanguageHandler() :
	applicationTranslator(NULL),
	systemTranslator(NULL)
{
}


LanguageHandler::~LanguageHandler() {
	// get rid of old translators
	// at this time, the application object doesn't live anylonger, so we
	// can savely destruct the translator objects
	delete applicationTranslator; applicationTranslator = NULL;
	delete systemTranslator; systemTranslator = NULL;
}


void LanguageHandler::setup(const QString & organization, const QString & program,
							const QString & translationDir, const QString & languageFilePrefix)
{
	m_organization = organization;
	m_program = program;
	m_translationDir = translationDir;
	m_languageFilePrefix = languageFilePrefix;
}


QString LanguageHandler::langId() {
	const char * const FUNC_ID = "[LanguageHandler::langId]";

	QSettings config(m_organization, m_program);
	QString langid = config.value("LangID", QString() ).toString();
	if (langid.isEmpty()) {
		// try to determine language id from OS
		QString localeName = QLocale::system().name();
				IBK::IBK_Message( IBK::FormatString("System locale: '%1'.\n").arg(localeName.toUtf8().data()), IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
				int pos = localeName.indexOf('_');
		if (pos != -1)
			localeName = localeName.left(pos);
		IBK::IBK_Message( IBK::FormatString("Translation required for locale: '%1'.\n").arg(localeName.toUtf8().data()),
			IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
		langid = localeName;

		if(langid.isEmpty())
			langid = "en";
	}
	return langid;
}


void LanguageHandler::setLangId(QString id) {
	QSettings config(m_organization, m_program);
	config.setValue("LangID", id );
}


void LanguageHandler::installTranslator(QString langId) {
	const char * const FUNC_ID = "[LanguageHandler::installTranslator]";

	// get rid of old translators
	if (applicationTranslator != NULL) {
		qApp->removeTranslator(applicationTranslator);
		delete applicationTranslator; applicationTranslator = NULL;
	}
	if (systemTranslator != NULL) {
		qApp->removeTranslator(systemTranslator);
		delete systemTranslator; systemTranslator = NULL;
	}


	// create new translators, unless we are using english
	if (langId == "en") {
		QSettings config(m_organization, m_program);
		config.setValue("LangID", langId);
		QLocale loc(QLocale::English);
		loc.setNumberOptions(QLocale::OmitGroupSeparator | QLocale::RejectGroupSeparator);
		QLocale::setDefault(loc);
		return;
	}


	IBK::IBK_Message( IBK::FormatString("Translation file path = '%1'.\n").arg(m_translationDir.toUtf8().data()),
		IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);

	systemTranslator = new QTranslator;

	// system translator first
	QString defaultTranslationFile = "qt_" + langId;
	if (systemTranslator->load(defaultTranslationFile, m_translationDir)) {
		qApp->installTranslator(systemTranslator);
		IBK::IBK_Message( IBK::FormatString("Installing system translator using file: '%1'.\n").arg(defaultTranslationFile.toUtf8().data()),
			IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
	}
	else {
		IBK::IBK_Message( IBK::FormatString("Could not load system translator file: '%1'.\n").arg(defaultTranslationFile.toUtf8().data()),
			IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
		// no translator found, remove it again
		delete systemTranslator;
		systemTranslator = NULL;
	}


	applicationTranslator = new QTranslator;
	if (applicationTranslator->load(m_languageFilePrefix + "_" + langId, m_translationDir)) {
		IBK::IBK_Message( IBK::FormatString("Installing application translator using file: '%1_%2'.\n")
						  .arg(m_languageFilePrefix.toUtf8().data()).arg(langId.toUtf8().data()),
			IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
		qApp->installTranslator(applicationTranslator);
		// remember translator in settings
		QSettings config(m_organization, m_program);
		config.setValue("LangID", langId);
	}
	else {
		IBK::IBK_Message( IBK::FormatString("Could not load application translator file: '%1_%2'.\n")
						  .arg(m_languageFilePrefix.toUtf8().data()).arg(langId.toUtf8().data()),
			IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
		delete applicationTranslator;
		applicationTranslator = NULL;
	}

	// now also set the corresponding locale settings (for number display etc.)
	if (langId == "de") {
		QLocale loc(QLocale::German);
		loc.setNumberOptions(QLocale::OmitGroupSeparator | QLocale::RejectGroupSeparator);
		QLocale::setDefault(loc);
	}
}

} // namespace QtExt
