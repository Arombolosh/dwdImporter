/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_DirectoriesH
#define QtExt_DirectoriesH

#include <QString>

namespace QtExt {

/*! Provides default locations for resource and user data files based on IBK_DEPLOYMENT
	flag setting and OS.

	Use this as base class for application-specific directory management.
*/
class Directories {
public:
	/*! This directory name is appended to all platform-specific locations.
		For example 'Delphin6' will generate a database save location: /home/<user>/.local/share/Delphin6
		or C:\Users\<user>\AppData\Roaming\Delphin6 on Windows.
	*/
	static QString appname;
	/*! The top-level directory within the source code directory tree, where resources and translations can be found.
		For example 'TheraklesApp' will be used to generate the translation dir: `resourcesRootDir() + "/../TheraklesApp/resources/translations"`
	*/
	static QString devdir;



	/*! Returns the platform-specific root directory of all read-only resource files. */
	static QString resourcesRootDir();

	/*! Returns the platform-specific directory of all read-only translation files. */
	static QString databasesDir();

	/*! Returns the platform-specific directory of all read-only translation files. */
	static QString translationsDir();

	/*! Returns the platform-specific root directory of all user database files. */
	static QString userDataDir();

	/*! Returns the platform-specific temporary directory. */
	static QString tmpDir();

	/*! Returns path to global log file. */
	static QString globalLogFile();

	/*! Generates a new temporary file path (file not yet existing) and tests writing into this file.
		If writing into file is not possible, an empty string is returned.
	*/
	static QString generateTempFileName();

	/*! This function opens the file in "write" mode and removes the file afterwards again to test if writing is possible.
		\return True, if file can be written into. False if the file already exists, or if file cannot be opened for writing.

		\note Function expects parent directory to exist.
	*/
	static bool checkForWriteAccess(const QString & newFileName);

	/*! Local file path to update-installer. */
	static QString updateFilePath();

	/*! Qt4 function for removing directory structures recursively, in Qt5 use QDir::removeRecursively(). */
	static bool removeDirRecursively(const QString & directory);
};

}

#endif // QtExt_DirectoriesH
