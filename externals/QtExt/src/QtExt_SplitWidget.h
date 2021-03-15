/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_SplitWidgetH
#define QtExt_SplitWidgetH

#include <QWidget>
#include <QSplitter>
#include <QSplitterHandle>

class QWidget;
class QSplitter;
class QVBoxLayout;


namespace QtExt {

/*! A manager class for handling widgets that can be "split" and "joined".
	This class manages a tree of widgets that is composed of splitters.

	\code
	// Initially the tree should receive a root widget
	splitWidget->setRootWidget(myWidget); // myWidget now belongs to splitWidget
	// the split widget manager now holds:
	// myWidget (root)

	// then, we can split the widget vertically, and insert the new widget on the left side
	splitWidget->split(myWidget, anotherWidget, Qt::Vertical, true);
	// the split widget manager now holds:
	// QSplitter:Vertical (root)
	//   - anotherWidget
	//   - myWidget

	// if we split the widget to the right again, we call
	splitWidget->split(myWidget, widgetThree, Qt::horizontal, false);
	// the split widget manager now holds:
	// QSplitter:Vertical (root)
	//   - anotherWidget
	//   - QSplitter:Horizontal
	//     - myWidget
	//     - widgetThree

	// we can remove a widget
	splitWidget->remove(anotherWidget);
	// QSplitter:Horizontal (root)
	//   - myWidget
	//   - widgetThree
	\endcode
*/
class SplitWidget : public QWidget {
	Q_OBJECT
public:
	SplitWidget(QWidget *parent);
	~SplitWidget();


	/*! Returns the root widget, may be NULL if no root widget has been set. */
	QWidget * rootWidget() { return m_rootWidget; }

	/*! Returns pointer to current root widget (may be NULL) and removes parent
		from widget so that it is no longer owned by SplitWidgetManager.
		After a call to this function the SplitWidgetManager does not have a root widget
		anylonger.
	*/
	QWidget * releaseRootWidget();

	/*! Sets/replaces root widget.
		If a root widget previously existed, it will be deleted (and all its children).
	*/
	void setRootWidget(QWidget * widget);

	/*! Returns the left/top widget in the splitter.
		This is a convenience function, even though the member function of QSplitter
		could be used instead as well.
		\note This function raises an Assertion if the parent does not have a child.
		\return Returns a child widget/splitter (you may have to cast QWidget to QSplitter to
				check this).
	*/
	QWidget * child(QSplitter * splitter, bool left);

	/*! Returns the nearest neighbor of a widget inside the split widget.
		If the widget is the root widget, the function returns NULL.
		The nearest neighbor widget is the widget that is found if the widget's parent splitter is taken
		as starting point and the tree is traversed taking always left widgets in a splitter, until
		a widget is found.
	*/
	QWidget * neighbor(QWidget * widget);

	/*! Returns the first non-QSplitter widget in a left/top sided recursive search through the tree. */
	QWidget * searchWidget(QSplitter * splitter);

	/*! Returns true if widget (may be a splitter) has children. */
	bool hasChilds(const QWidget * widget) const;

	/*! Splits a widget into two.
		\param parentWidget The widget that should be split into two.
		\param newWidget The widget that is inserted besides parentWidget
		\param orientation The split direction.
		\param left Insert new widget either to the left/top or right/bottom of parentWidget.
	*/
	void split(QWidget * parentWidget, QWidget * newWidget, Qt::Orientation orientation, bool left);

	/*! Removes a widget.
		\note Widget is unparented and deleted.
	*/
	void removeWidget(QWidget * widget);

private:
	QWidget		*m_rootWidget;
	QVBoxLayout	*m_layout;
};

/*! Custom splitter handle. */
class SplitWidgetHandle : public QSplitterHandle {
	Q_OBJECT
public:
	SplitWidgetHandle(Qt::Orientation o, QSplitter *parent);

protected:
	virtual void paintEvent(QPaintEvent *event);

};

/*! Custom splitter with own splitter handle for our split widget. */
class SplitWidgetSplitter : public QSplitter {
	Q_OBJECT
public:
	SplitWidgetSplitter(Qt::Orientation orientation, QWidget *parent = 0) :
		QSplitter(orientation, parent)
	{}

protected:
	QSplitterHandle *createHandle();
};


} // namespace QtExt

#endif // QtExt_SplitWidgetH
