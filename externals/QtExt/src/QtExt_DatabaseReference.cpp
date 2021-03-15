/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_DatabaseReference.h"

#include <QStringList>

namespace QtExt {

DatabaseReference::DatabaseReference() :
	m_location(LocalDirectory)
{
}

DatabaseReference::DatabaseReference(DatabaseLocation loc, const QString & typeStr,
									 const QString & desc, const QUrl & url) :
	m_location(loc),
	m_typeString(typeStr),
	m_description(desc),
	m_url(url)
{
}


QString DatabaseReference::toString() const {
	// encode database reference in format:
	// [LOCAL_DIRECTORY|Material|Default Material Database] file://path/to/database/directory
	QString encoded = "[";
	switch (m_location) {
		case LocalDirectory		: encoded += "LOCAL_DIRECTORY|"; break;
		case Webservice			: encoded += "WEBSERVICE|"; break;
		default					: encoded += "UNKNOWN|"; break;
	}
	encoded += m_typeString + "|" + m_description + "] " + m_url.toString();
	return encoded;
}


DatabaseReference DatabaseReference::fromString(const QString & str) {
	DatabaseReference dbRef;
	// split string into parts
	int pos2 = str.indexOf("]");
	int pos1 = str.indexOf("[");
	if (pos2 < pos1 || pos1 != 0 || pos2 == -1) {
		return dbRef; // invalid reference
	}
	QString url = str.right(str.size() - pos2 - 1);
	dbRef.m_url = QUrl(url.trimmed());
	QString remainder = str.mid(1, pos2-1);
	QList<QString> tokens = remainder.split("|");
	if (tokens.size() != 3) {
		return dbRef;
	}
	if (tokens[0] == "LOCAL_DIRECTORY")
		dbRef.m_location = LocalDirectory;
	else if (tokens[0] == "WEBSERVICE")
		dbRef.m_location = Webservice;
	else return dbRef;
	dbRef.m_typeString = tokens[1];
	dbRef.m_description = tokens[2];
	return dbRef;
}


} // namespace QtExt
