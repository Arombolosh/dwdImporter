/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_TableWidgetH
#define QtExt_TableWidgetH

#include <QWidget>

/*! \brief Namespace for Qt Extension lib.*/
/*! The namespace QtExt includes all classes and functions for Qt extensions.
*/
namespace QtExt {

	class Table;

/*! \brief Class TableWidget allows to create and draw a table with HTML-formated text.*/
class TableWidget : public QWidget
{
Q_OBJECT
public:
	/*! Default constructor.*/
	explicit TableWidget(QtExt::Table* table, QWidget *parent = 0);
	Table* table();

protected:
	virtual void paintEvent ( QPaintEvent * event );	///< Paint event. Calls drawTable.
	virtual void resizeEvent ( QResizeEvent * event );	///< Rezize event. Does nothing.

signals:

public slots:
	void repaintTable();

private:
	Table* m_table;

};


/*! @file QtExt_TableWidget.h
  @brief Contains the declaration of the class TableWidget.
*/

} // namespace QtExt
#endif // QtExt_TableWidgetH
