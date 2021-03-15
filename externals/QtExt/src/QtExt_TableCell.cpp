/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_TableCell.h"

namespace QtExt {

TableCell::TableCell(qreal width) :
		m_leftBorderWidth(width),
		m_rightBorderWidth(width),
		m_topBorderWidth(width),
		m_bottomBorderWidth(width),
		m_leftMargin(0),
		m_rightMargin(0),
		m_topMargin(0),
		m_bottomMargin(0),
		m_backgroundColor(Qt::transparent),
		m_merged(false),
		m_verticalOffset(0.0),
		m_verticalText(false)
{
}

TableCell::TableCell(const TableCell& src)  :
	m_leftBorderWidth(src.m_leftBorderWidth),
	m_rightBorderWidth(src.m_rightBorderWidth),
	m_topBorderWidth(src.m_topBorderWidth),
	m_bottomBorderWidth(src.m_bottomBorderWidth),
	m_leftMargin(src.m_leftMargin),
	m_rightMargin(src.m_rightMargin),
	m_topMargin(src.m_topMargin),
	m_bottomMargin(src.m_bottomMargin),
	m_alignment(src.m_alignment),
	m_text(src.m_text),
	m_textRect(src.m_textRect),
	m_maxTextRect(src.m_maxTextRect),
	m_cellRect(src.m_cellRect),
	m_backgroundColor(src.m_backgroundColor),
	m_merged(src.m_merged),
	m_verticalOffset(src.m_verticalOffset),
	m_textInfos(src.m_textInfos),
	m_verticalText(src.m_verticalText)
{
}

TableCell& TableCell::operator=(TableCell src) {
	Swap(src);
	return *this;
}

void TableCell::Swap(TableCell& src) {
	std::swap(m_leftBorderWidth, src.m_leftBorderWidth);
	std::swap(m_rightBorderWidth, src.m_rightBorderWidth);
	std::swap(m_topBorderWidth, src.m_topBorderWidth);
	std::swap(m_bottomBorderWidth, src.m_bottomBorderWidth);
	std::swap(m_leftMargin, src.m_leftMargin);
	std::swap(m_rightMargin, src.m_rightMargin);
	std::swap(m_topMargin, src.m_topMargin);
	std::swap(m_bottomMargin, src.m_bottomMargin);
	std::swap(m_alignment, src.m_alignment);
	std::swap(m_text, src.m_text);
	std::swap(m_textRect, src.m_textRect);
	std::swap(m_maxTextRect, src.m_maxTextRect);
	std::swap(m_cellRect, src.m_cellRect);
	std::swap(m_backgroundColor, src.m_backgroundColor);
	std::swap(m_merged, src.m_merged);
	std::swap(m_verticalOffset, src.m_verticalOffset);
	std::swap(m_textInfos, src.m_textInfos);
	std::swap(m_verticalText, src.m_verticalText);
}

void TableCell::setBorderWidth(TableCell::CellBorder border, qreal width) {
	Q_ASSERT(width >= 0);

	if( border & LeftBorder)
		m_leftBorderWidth = width;
	if( border & RightBorder)
		m_rightBorderWidth = width;
	if( border & TopBorder)
		m_topBorderWidth = width;
	if( border & BottomBorder)
		m_bottomBorderWidth = width;
}

qreal TableCell::borderWidth(TableCell::CellBorder border) const {
	switch(border) {
		case LeftBorder: return m_leftBorderWidth;
		case RightBorder: return m_rightBorderWidth;
		case TopBorder: return m_topBorderWidth;
		case BottomBorder: return m_bottomBorderWidth;
		default: return 0;
	}
}

void TableCell::setAlignment(Qt::Alignment alignment) {
	m_alignment = alignment;
}

Qt::Alignment TableCell::alignment() const {
	return m_alignment;
}

void TableCell::setText(const QString& text) {
	m_text = text;
}

QString TableCell::text() const {
	return m_text;
}

QRectF TableCell::textRect() const {
	return m_textRect;
}

void TableCell::setTextRect(const QRectF& rect) {
	m_textRect = rect;
}

QRectF TableCell::maxTextRect() const {
	return m_maxTextRect;
}

void TableCell::setMaxTextRect(const QRectF& rect) {
	m_maxTextRect = rect;
}

QSizeF TableCell::textSize(qreal maxWidth) {
	QSizeF size = m_textInfos.sizeForMaximumWidth(maxWidth).size();
	if(m_verticalText)
		size.transpose();
	return size;
}

qreal TableCell::textMaxWidth(qreal maxWidth) {
	return m_textInfos.sizeForMaximumWidth(maxWidth).m_maxWidth;
}

QRectF TableCell::cellRect() const {
	return m_cellRect;
}

void TableCell::setCellRect(const QRectF& rect) {
	m_cellRect = rect;
}

QColor TableCell::backgroundColor() const {
	return m_backgroundColor;
}

void TableCell::setBackgroundColor(const QColor& col) {
	m_backgroundColor = col;
}

qreal TableCell::leftMargin() const {
	return m_leftMargin;
}

void TableCell::setLeftMargin(qreal margin) {
	m_leftMargin = margin;
}

qreal TableCell::rightMargin() const {
	return m_rightMargin;
}

void TableCell::setRightMargin(qreal margin) {
	m_rightMargin = margin;
}

qreal TableCell::topMargin() const {
	return m_topMargin;
}

void TableCell::setTopMargin(qreal margin) {
	m_topMargin = margin;
}

qreal TableCell::bottomMargin() const {
	return m_bottomMargin;
}

void TableCell::setBottomMargin(qreal margin) {
	m_bottomMargin = margin;
}

qreal TableCell::verticalOffset() const {
	return m_verticalOffset;
}

void TableCell::setVerticalOffset(qreal offset) {
	m_verticalOffset = offset;
}

void TableCell::setMerged(bool merged) {
	m_merged = merged;
}

void TableCell::setVerticalText(bool vertical) {
	m_verticalText = vertical;
}

void TableCell::setPaintProperties(QTextDocument* textDocument) {
	m_textInfos.set(textDocument, m_text);
}


} // namespace QtExt
