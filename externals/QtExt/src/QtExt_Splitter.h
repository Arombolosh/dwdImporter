/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_SplitterH
#define QtExt_SplitterH

#include <QSplitter>
#include <QMap>
#include <QWidget>
#include <QPair>

class QHBoxLayout;
class QVBoxLayout;

namespace QtExt {

	class FeatureWidget;


/*! Implements a spezialised splitter.

	Widgets in the splitter can be collapsed or expanded, controlled via
	setWidgetCollapsed() member function. When a widget is collapsed only
	a small part of it remains visible.	The other widgets are then resized
	accordingly.

	The widget also holds a dummy widget which is shown when
	all normal widgets are collapsed.
*/
class Splitter : public QWidget {
	Q_OBJECT
public:
	/*! The wrapper constructor. */
	explicit Splitter(QWidget* parent = 0);
	/*! The wrapper constructor. */
	explicit Splitter(Qt::Orientation, QWidget* parent = 0);
	~Splitter();

	/*! The wrapper function to add a widget.
		The widget is visible by default.
	*/
	void addWidget ( QWidget * widget );
	/*! The wrapper function to insert or move a widget within the splitter.
		Inserts a widget at a given location.
	*/
	void insertWidget ( int index, QWidget * widget );

	/*! Toggles collapsed state of widgets. */
	void setWidgetCollapsed(QWidget * w, bool collapsed);

public slots:

	/*! Slot should be connected to a FeatureWidget's ShadeButton, when it has collapsed the widget.
		See SIGNAL QtExt::FeatureWidget::collapsed( QtExt::FeatureWidget* );
	*/
	void resizeWidgetsCollapsed( QtExt::FeatureWidget * widget );
	/*! Slot should be connected to a FeatureWidget's ShadeButton, when it has expanded the widget.
		See SIGNAL QtExt::FeatureWidget::expanded( QtExt::FeatureWidget* );
	*/
	void resizeWidgetsExpand( QtExt::FeatureWidget * widget );

private:

	/*! Holds initial setup stuff, called from constructors. */
	void setup( Qt::Orientation orientation);

	/*! Returns the index of the widget if it already exists in the splitter, or -1 if
		it does not exist yet.
	*/
	int widgetIndex(QWidget * w);

	/*! Resizes the splitter widgets based on the collapsed state. */
	void adjustSizes();

	/*! Recalculate the widget proportions. */
	void resizeWidgets( QtExt::FeatureWidget * widget, bool hideState );

	/*! Adds the collapsed state to map and list. */
	void setHide( QWidget * widget, bool hideState, int index = -1 );

	/*! The private splitter instance. */
	QSplitter							*m_splitter;
	/*! This layout is used for vertical splitter. */
	QVBoxLayout							*m_vBLayout;
	/*! This layout is used for horizontal splitter. */
	QHBoxLayout							*m_hBLayout;
	/*! The dummy widget. This workaround force all gribs to be "disabled" when all feature widgets are collapsed. */
	QWidget								*m_dummy;
	/*! This list stores false for every widget in the splitter that is in "Collapsed" state. */
	QList<bool>							m_collapsedWidgets;
};

} // namespace QtExt

#endif // QtExt_SplitterH
