/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_ColorListModel.h"

#include <QColor>

namespace QtExt {

const int NUM_DEFAULT_COLORS = 16;
const QColor DEFAULT_COLORS[NUM_DEFAULT_COLORS] = {
	QColor( "#416fce"),
	QColor( "#ffaa00"),
	QColor( "#ff007f"),
	QColor( "#7141d1"),
	QColor( "#ffee00"),
	QColor( "#45ad45"),
	QColor( "#aa0000"),
	QColor( "#55aaff"),
	QColor( "#005500"),
	QColor( "#550000"),
	QColor( "#ffaaff"),
	QColor( "#c66300"),
	QColor( "#ff5500"),
	QColor( "#aa55ff"),
	QColor( "#aaaa00"),
	QColor( "#ffffff")
};

int ColorListModel::m_colorCounter = 0;

ColorListModel::ColorListModel(QObject *parent) :
	QAbstractListModel(parent)
{
	// set default colors for db
	for (int i=0; i<NUM_DEFAULT_COLORS; ++i)
		m_dbColors.append(DEFAULT_COLORS[i]);

	addColors();
}

int ColorListModel::rowCount(const QModelIndex &/*parent*/) const {
	return m_colors.size();
}

QVariant ColorListModel::data(const QModelIndex &index, int role) const {

	if (!index.isValid())
		return QVariant();

	if (role == Qt::BackgroundRole) {
		int pos = ( index.row() + m_dbColors.count() ) % m_dbColors.count();
		return QColor(m_colors.at( pos ));
	}

	if (role == ColorListModel::ColorRole)
		return QColor(m_colors.at(index.row()));

	if (role == Qt::DisplayRole)
		return QString(" ");
	else
		return QVariant();
}

QModelIndex ColorListModel::index ( int row, int column, const QModelIndex & /*parent*/ ) const {

	return createIndex(row,column);
}

void ColorListModel::addColors() {

	m_colors.clear();
	// add default and usercolors from settings
	m_colors.append( m_dbColors );
}

void ColorListModel::appendUserColor( QColor color ){
	m_colors.push_back( color );
}

void ColorListModel::appendUserColor( QList< QColor > evenMoreColor ){
	m_colors.append( evenMoreColor );
}

} // namespace PPP
