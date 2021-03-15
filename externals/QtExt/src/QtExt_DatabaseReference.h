/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_DatabaseReferenceH
#define QtExt_DatabaseReferenceH

#include <QString>
#include <QUrl>

namespace QtExt {

/*! This class references a database (directory). */
class DatabaseReference {
public:

	/*! The database location. */
	enum DatabaseLocation {
		/*! Standard type of data source, materials are located in a directory. */
		LocalDirectory,
		/*! Special data source type, is only populated temporary when material files
			are stored relative to the project file. */
		DirectoryRelativeToProjectFile,
		/*! Data is provided through web-service-query. */
		Webservice
	};

	/*! Default constructor, creates an empty reference to a LocalDirectory db. */
	DatabaseReference();

	/*! Initializing constructor.
		\param loc Location of the database, either as a standard materials are located in a directory, or as a special data source type, is only populated temporary when material files are stored relative to the project file.
		\param typeStr A string describing the type of the database, for example "Material", or "Construction".
		\param desc A description shown to the user.
		\param url URL pointing to the database/service.
	*/
	DatabaseReference(DatabaseLocation loc, const QString & typeStr,
		const QString & desc, const QUrl & url);

	/*! Returns a string encoding the database reference. */
	QString toString() const;

	/*! Creates a database reference object from an encoded string. */
	static DatabaseReference fromString(const QString & str);



	/*! Location of the database. */
	DatabaseLocation			m_location;
	/*! A string describing the type of the database, for example "Material", or "Construction". */
	QString						m_typeString;
	/*! A description shown to the user. */
	QString						m_description;
	/*! URL pointing to the database/service. */
	QUrl						m_url;

};


} // namespace QtExt


#endif // QtExt_DatabaseReferenceH
