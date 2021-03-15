/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_DoubleWidgetBaseH
#define QtExt_DoubleWidgetBaseH

#include <QWidget>
#include <QHBoxLayout>

namespace QtExt {


/*! \brief Widget base class to display two widgets and toggle the visibility of both.

	DoubleWidgetBase includes two widgets and sets the default layout properties.
	One of both widget can be displayed. DoubleWidgetBase includes a toggle function
	to set the visibility to the other widget. You have to give the specific widget
	in the constructor of this class.
*/
class DoubleWidgetBase : public QWidget {
	Q_OBJECT
public:
	/*! Constructor.
		\param widget1 First widget.
		\param widget2 Second widget.
		\param parent Parent widget.
		The class sets the ownership of both widget to the parent of this class.
		The widget should not be deleted externally.
	*/
	DoubleWidgetBase(QWidget* widget1, QWidget* widget2, QWidget *parent);

	/*! Default destructor. */
	~DoubleWidgetBase();

	/*! Returns true if the first widget is visible and false if the second widget is visible. */
	bool isFirstVisible() { return m_widget1->isVisibleTo(this); }

public slots:
	/*! Toggels the visibility of the two widgets and emits widgetsChanged(). */
	void toggleWidgets();

	/*! Sets the visibility of the first widget.
		The visibility of the second one will be set to the contrary.
		The signal widgetsChanged() is only emitted when the visible of the
		widgets has changed.
		\param visible Visibility of the first widget.
	*/
	void setFirstVisible(bool visible);

signals:
	/*! Fires when the widgets has changed its visibility. */
	void widgetsChanged();

protected:
	/*! Widget to display plot data. */
	QWidget		*m_widget1;

	/*! Widget to display data. */
	QWidget		*m_widget2;
};

} // namespace QtExt

#endif // QtExt_DoubleWidgetBaseH
