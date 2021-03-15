/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_ColorEditWidgetH
#define QtExt_ColorEditWidgetH

#include <QWidget>
#include <QColor>

class QLabel;
class QPushButton;
class QFrame;

namespace QtExt {

/*! A widget that encapsulated a label, a color widget and a push button. */
class ColorEditWidget : public QWidget {
	Q_OBJECT
public:
	/*! Default constructor. */
	ColorEditWidget(QWidget * parent);
	/*! Overloaded constgructor.
		\param label Label for description.
		\param parent Parent widget.
	*/
	ColorEditWidget(const QString & label, QWidget * parent = 0);
	/*! Sets the label for the widget. */
	void setLabel(const QString & label);
	/*! Sets the current color. */
	void setColor(const QColor & color);
	/*! Returns the current color. */
	QColor color() const;

signals:
	/*! Emitted when user has just selected a new color.
		This signal is _not_ emitted when the color is set via setColor(). */
	void colorSelected(QColor);

private slots:
	/*! Triggered when user presses edit button. */
	void onSelectColor();

private:
	void setupUi();

	QLabel		*m_label;		///< Internal label.
	QFrame		*m_colorWidget;		///< Frame that presents the color.
	QPushButton	*m_pushButton;		///< Button to start a color dialog.
};

/*! \file QtExt_ColorEditWidget.h
	\brief Contains the declaration of the widget ColorEditWidget.
*/

} // namespace QtExt
#endif // QtExt_ColorEditWidgetH
