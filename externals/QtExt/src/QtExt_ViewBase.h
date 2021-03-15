/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_ViewBaseH
#define QtExt_ViewBaseH

#include <QWidget>

class QPushButton;
class QShortcut;
class QKeysequence;
class QSettings;
class QSplitter;

namespace QtExt {

class PanelButton;
class PushButton;

/*! Base class for all views.
	This class handles layout creation for panel widgets and creates associated
	panel buttons. It also handles saving/restoring of the view configuration.
*/
class ViewBase : public QWidget {
	Q_OBJECT
public:

	/*! Constructor.*/
	explicit ViewBase(QWidget * parent, const QKeySequence keySequence);

	/*! Destructor for debugging purposes.*/
	~ViewBase();

	/*! Initializes layout, takes pointers to panels.
		Pass NULL pointers to not existing panels.
		\todo Replace QWidget with StatePanel (class providing functionality for saving and restoring usage states).
	*/
	void initPanels(QWidget * centerPanel, QWidget * westPanel = NULL,
					QWidget * eastPanel = NULL, QWidget * southPanel = NULL,
					bool spHasEastButton = false, bool spHasWestButton = false);

	/*! Returns a button that can be used to show the view.
		The button that is returned is parented to the respective view.
		Re-parent the button if you want to transfer ownership, but make sure that
		the new owner exists as long as the viewButton() function is accessed.
	*/
	PushButton * viewButton() const { return m_viewButton; }

	/*! Returns the assigned short cut for this view button. */
	QShortcut * shortcut() const { return m_shortcut; }

	/*! Sets the panel button enabled/disabled state.
		\param panelWidget Pointer to panel widget (passed to initPanels()) function
		\param visible If true, panel-button is enabled. If false, panel-button is disabled.
		\note This function does not change visibility of the corresponding panel widget.
	*/
	void setPanelButtonEnabled(QWidget * panelWidget, bool visible);

	/*! This function hides/shows a panel widget and modifies the checked state
		of the appropriate panel button.
		\param panelWidget Pointer to panel widget (passed to initPanels()) function
		\param visible If true, panel is made visible and panel-button state is switched accordingly.
	*/
	void setPanelVisible(QWidget * panelWidget, bool visible);

	/*! Stores the display and enabled states to the given settings.
		Default implementation just stores states of panels and corresponding buttons.
		Re-implement in each view to store view-specific settings.
		\warning Don't forget to call ViewBase::writeSettings() in reimplementations.
	*/
	virtual void writeSettings( QSettings& settings );

	/*! Restores the display and enabled states from the given settings.
		Default implementation just restores states of panels and corresponding buttons.
		Re-implement in each view to restore view-specific settings.
		\warning Don't forget to call ViewBase::readSettings() in reimplementations.
	*/
	virtual void readSettings( QSettings& settings );

signals:
	/*! Emitted when south panel east button has been checked. */
	void spEastButtonChecked(bool checked);
	/*! Emitted when south panel west button has been checked. */
	void spWestButtonChecked(bool checked);

public slots:
	/*!
		This slot will be triggered from outside application when program is closed.
		It allows an order in which a sub component and its master application save its
		contents. Needs to be reimplemented if neccessary by subcomponent.
	*/
	void saveAllOpenSubContents();

private slots:
	void on_westPanelButton_Clicked();
	void on_eastPanelButton_Clicked();
	void on_southPanelButton_Clicked();
	void on_spEastButton_Clicked();
	void on_spWestButton_Clicked();

private:
	/*! The view button is created in the construction.
		Adjust the caption manually in the deriving class.
	*/
	PushButton * m_viewButton;
	QShortcut * m_shortcut;

	/*! The panel buttons hide and show the directed panels.
	 */
	PanelButton * m_epButton;
	PanelButton * m_wpButton;
	PanelButton * m_spButton;

	QWidget * m_centerPanel;
	QWidget * m_westPanel;
	QWidget * m_eastPanel;
	QWidget * m_southPanel;

	PanelButton * m_spEastButton;
	PanelButton * m_spWestButton;

	/*! The horizontal splitter that holds the west panel, center panel and east panel. */
	QSplitter * m_horizontalSplitter;
	int			m_lastWestPanelSize;
	int			m_lastEastPanelSize;

	/*! The vertical splitter holds top and bottom part widgets, whereas top part holds the
		horizontal splitter.
	*/
	QSplitter * m_verticalSplitter;
	QWidget*	m_widgetTopPart;
	QWidget*	m_widgetBottomPart;
};

} // namespace QtExt

#endif // QtExt_ViewBaseH
