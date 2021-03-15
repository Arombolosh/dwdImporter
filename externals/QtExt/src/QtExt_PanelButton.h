/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_PanelButtonH
#define QtExt_PanelButtonH

#include <QToolButton>

class QWidget;



namespace QtExt {


/*! The shade/panel button that is used to hide/show the west/east/north/south panels.
	The panel button has two states:
	* checked, the corresponding panel widget is visible and the arrow points outwards
	* unchecked, the corresponding panel widget is invisible and the arrow points inwards
*/
class PanelButton : public QToolButton {
	Q_OBJECT
public:

	/*! The possible directions/alignments of the panel button within the GUI. */
	enum Direction{
		DD_NORTH,	///< Orientation north, horziontal upper edge of the screen.
		DD_SOUTH,	///< Orientation south, horziontal lower edge of the screen.
		DD_EAST,	///< Orientation east, vertical right edge of the screen.
		DD_WEST,	///< Orientation west, vertical left edge of the screen.
	};

	/*! Constructor, takes a \a parent and the panel button \a direction. */
	explicit PanelButton( QWidget *parent, Direction direction );

	/*! Makes run-time switching of international shortcuts possible */
	void retranslateUi();

private:

	/*! Holds the direction/location of the button.
		\sa Direction
	*/
	Direction m_direction;
};

} // namespace QtExt


#endif // QtExt_PanelButtonH
