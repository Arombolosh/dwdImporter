/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_ColorListModelH
#define QtExt_ColorListModelH

#include <QAbstractListModel>
#include <QList>
#include <QColor>

namespace QtExt {


/*! \brief The class ColorListModel is a model for a combobox.
*/
class ColorListModel : public QAbstractListModel
{
	Q_OBJECT
public:
	/*!  Custom item roles supported by this model.	*/
	enum CustomItemRole {
		ColorRole	  = Qt::UserRole + 1		///< Role for color of the item.
	};

	explicit ColorListModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	/*! Returns the index for the actual item including a internalPointer to the data item. */
	virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;

	static int	m_colorCounter;

	/*! Append all user given colors. */
	void appendUserColor( QColor );

	/*! Append all user given colors. */
	void appendUserColor( QList< QColor > );

	/*! Set a new color count.*/
	void setColorCount(unsigned int count);

private:

	/*! Add predefined colors to the color list. */
	void addColors();

	/*! Holds all predefined colors. s*/
	QList<QColor>	m_colors;

	/*! User colors used in db views.*/
	QList< QColor >	m_dbColors;

};

} // namespace QtExt

#endif // QtExt_ColorListModelH
