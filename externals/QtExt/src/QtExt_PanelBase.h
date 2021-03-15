/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_PanelBaseH
#define QtExt_PanelBaseH

#include <QScrollArea>

class QSettings;

namespace QtExt {

/*! Base class for all panels. */
class PanelBase : public QScrollArea {
	Q_OBJECT
public:
	/*! Constructor. */
	explicit PanelBase(QWidget * w);
	/*! Destructor. */
	virtual ~PanelBase() {}

	/*! Stores the display and enabled states to the given settings.
		Re-implement in each view to store view-specific settings.
	*/
	virtual void writeSettings( QSettings& settings ) = 0;

	/*! Restores the display and enabled states from the given settings.
		Re-implement in each view to restore view-specific settings.
	*/
	virtual void readSettings( QSettings& settings ) = 0;
};

} // namespace QtExt {


#endif // QtExt_PanelBaseH
