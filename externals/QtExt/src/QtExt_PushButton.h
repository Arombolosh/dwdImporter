/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_PushButtonH
#define QtExt_PushButtonH

#include <QPushButton>

class QKeySeqence;

namespace QtExt {


/*! Styled view switch button.
	Emits signal controlClicked() when user clicks on the button while holding the CTRL key.
*/
class PushButton : public QPushButton {
	Q_OBJECT
public:
	explicit PushButton ( QWidget * parent = 0 );

signals:
	/*! Emitted when user clicks on the button while holding control key. */
	void controlClicked();


protected:
	virtual void	mousePressEvent ( QMouseEvent * e );

};

} // namespace QtExt {


#endif // QtExt_PushButtonH
