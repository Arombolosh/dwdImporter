/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include <numeric>
#include <algorithm>

#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QDebug>
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QAbstractTextDocumentLayout>
#include "QtExt_Table.h"

namespace QtExt {

Table::Table(QTextDocument* textDocument, QSize size, QObject *parent) :
	QObject(parent),
	m_textDocument(textDocument),
	m_textDocumentOwner(false),
	m_cols(0),
	m_rows(0),
	m_spacing(2),
	m_margin(2),
	m_outerFrameWidth(0.0),
	m_innerFrameWidth(1.0),
	m_background(Qt::transparent),
	m_size(size),
	m_scale(1.0),
	m_adjusted(false)
{
	if( m_textDocument == nullptr) {
		m_textDocument = new QTextDocument;
		m_textDocumentOwner = true;
	}
}

Table::~Table() {
	if(m_textDocumentOwner)
		delete m_textDocument;
}

void Table::clear() {
	m_cells.clear();
	m_HLines.clear();
	m_VLines.clear();
	m_columnWidths.clear();
	m_rowHeights.clear();
	m_adjusted = false;
	m_cols = 0;
	m_rows = 0;
	m_scale = 1.0;
	m_spacing = 2;
	m_margin = 2;
	m_outerFrameWidth = 0.0;
	m_innerFrameWidth = 1.0;
	m_background = QColor(Qt::transparent);
	m_textDocument->clear();
	m_size = QSize();
	m_mergedCells.clear();
}

void Table::setTableSize(QSize size) {
	// same size - no adjustment necessary
	if( m_size == size)
		return;

	m_size = size;
	m_adjusted = false;
	emit changed();
}

void Table::setColumnsRows(unsigned int cols, unsigned int rows) {
	// no change - no adjustment necessary
	if( m_cols == cols && m_rows == rows)
		return;

	m_cols = cols;
	m_rows = rows;

	std::vector<std::vector<TableCell> >(cols, std::vector<TableCell>(rows, TableCell(m_innerFrameWidth))).swap(m_cells);
	m_HLines = QVector<QVector<LineProperties> >(cols , QVector<LineProperties>(rows + 1));
	m_VLines = QVector<QVector<LineProperties> >(cols + 1, QVector<LineProperties>(rows));
	m_columnWidths.resize(cols);
	m_rowHeights = QVector<CellSizeFormater>(rows, CellSizeFormater(CellSizeFormater::AutoMinimum));
	m_adjusted = false;
//	emit changed();
}

void Table::setCellText(unsigned int col, unsigned int row, const QString& text) {
	Q_ASSERT(col < m_cols);
	Q_ASSERT(row < m_rows);

	m_cells[col][row].setText(text);
	m_adjusted = false;
	emit changed();
}

TableCell& Table::cell(unsigned int col, unsigned int row) {
	Q_ASSERT(col < m_cols);
	Q_ASSERT(row < m_rows);

	m_adjusted = false;
	return m_cells[col][row];
}

const TableCell& Table::cell(unsigned int col, unsigned int row) const {
	Q_ASSERT(col < m_cols);
	Q_ASSERT(row < m_rows);

	return m_cells[col][row];
}

void Table::setSpacing(qreal spacing) {
	if( m_spacing != spacing) {
		m_spacing = spacing;
		m_adjusted = false;
		emit changed();
	}
}

qreal Table::margin() const {
	return m_margin;
}

void Table::setMargin(qreal margin) {
	if( m_margin != margin) {
		m_margin = margin;
		m_adjusted = false;
		emit changed();
	}
}

qreal Table::spacing() const {
	return m_spacing;
}

void Table::setBackgroundColor(const QColor& col, bool allCells) {
	if( m_background != col) {
		m_background = col;
	}
	if( allCells ) {
		for( unsigned int r=0; r<m_rows; ++r){
			for( unsigned int c=0; c<m_cols; ++c) {
				m_cells[c][r].setBackgroundColor(col);
			}
		}
	}
}

QColor Table::backgroundColor() const {
	return m_background;
}

int Table::mergeIndex(unsigned int col, unsigned int row) const {
	Q_ASSERT(col < m_cols);
	Q_ASSERT(row < m_rows);

	for( int i=0, count = m_mergedCells.size(); i<count; ++i) {
		const MergedCells& mc = m_mergedCells[i];
		if( col >= mc.firstCol && col <= mc.lastCol && row >= mc.firstRow && row <= mc.lastRow)
			return i;
	}
	return -1;
}

bool Table::multiColMerge(unsigned int col, unsigned int row) const {
	Q_ASSERT(col < m_cols);
	Q_ASSERT(row < m_rows);

	int index = mergeIndex(col, row);
	if( index == -1)
		return false;

	// no check for index necessary because mergeIndex cannot create a wrong one
	const MergedCells& mc = m_mergedCells[index];
	return mc.lastCol > mc.firstCol;
}

bool Table::multiRowMerge(unsigned int col, unsigned int row) const {
	Q_ASSERT(col < m_cols);
	Q_ASSERT(row < m_rows);

	int index = mergeIndex(col, row);
	if( index == -1)
		return false;

	// no check for index necessary because mergeIndex cannot create a wrong one
	const MergedCells& mc = m_mergedCells[index];
	return mc.lastRow > mc.firstRow;
}

bool Table::mergeCells(unsigned int col, unsigned int row, unsigned int numCols, unsigned int numRows) {
	Q_ASSERT(numCols > 1 || numRows > 1);

	const unsigned int lastCol = col + numCols - 1;
	const unsigned int lastRow = row + numRows - 1;
	if( lastCol >= m_cols || lastRow >= m_rows)
		return false;

	for( unsigned int j=row; j<row + numRows; ++j){
		for( unsigned int i=col; i<col + numCols; ++i) {
			if( m_cells[i][j].merged())
				return false;
		}
	}
	for( unsigned int j=row; j<row + numRows; ++j){
		for( unsigned int i=col; i<col + numCols; ++i) {
			m_cells[i][j].setMerged(true);
		}
	}
	m_mergedCells.push_back(MergedCells(col, row, col + numCols - 1, row + numRows - 1));
	m_mergedCells.back().m_verticalText = m_cells[col][row].verticalText();
	m_adjusted = false;
	return true;
}

qreal Table::columnSize(unsigned int col) const {
	Q_ASSERT(col < m_cols);

	return m_columnWidths[col].m_size;
}

qreal Table::rowSize(unsigned int row) const {
	Q_ASSERT(row < m_rows);

	return m_rowHeights[row].m_size;
}

CellSizeFormater::FormatType Table::columnSizeFormat(unsigned int col) const {
	Q_ASSERT(col < m_cols);

	return m_columnWidths[col].m_format;
}

CellSizeFormater::FormatType Table::rowSizeFormat(unsigned int row) const {
	Q_ASSERT(row < m_rows);

	return m_rowHeights[row].m_format;
}

void Table::setColumnSizeFormat(unsigned int col, CellSizeFormater::FormatType format, qreal fixedSize) {
	Q_ASSERT(col < m_cols);
	// currently only these two types are supported
	if(format == CellSizeFormater::TableWidth || format == CellSizeFormater::Unknown)
		return;

	if( m_columnWidths[col].m_format != format || format == CellSizeFormater::Fixed) {
		m_columnWidths[col].m_format = format;
		m_adjusted = false;
		if( format == CellSizeFormater::Fixed)
			m_columnWidths[col].m_fixedSize = fixedSize;
		emit changed();
	}
}

void Table::setRowSizeFormat(unsigned int row, CellSizeFormater::FormatType format, qreal fixedSize) {
	Q_ASSERT(row < m_rows);
	// currently only these two types are supported
	if(format == CellSizeFormater::TableWidth || format == CellSizeFormater::Unknown)
		return;

	if( m_rowHeights[row].m_format != format || format == CellSizeFormater::Fixed) {
		m_rowHeights[row].m_format = format;
		m_adjusted = false;
		if( format == CellSizeFormater::Fixed)
			m_rowHeights[row].m_fixedSize = fixedSize;
		emit changed();
	}
}

void Table::setColumnMargins(unsigned int col, qreal leftMargin, qreal rightMargin) {
	Q_ASSERT( col < m_cols);

	for( unsigned int r=0; r<m_rows; ++r){
		if( leftMargin >= 0)
			m_cells[col][r].setLeftMargin(leftMargin);
		if( rightMargin >= 0)
			m_cells[col][r].setRightMargin(rightMargin);
	}
}

void Table::setRowMargins(unsigned int row, qreal topMargin, qreal bottomMargin) {
	Q_ASSERT( row < m_rows);

	for( unsigned int c=0; c<m_cols; ++c){
		if( topMargin >= 0)
			m_cells[c][row].setTopMargin(topMargin);
		if( bottomMargin >= 0)
			m_cells[c][row].setBottomMargin(bottomMargin);
	}
}

void Table::setOuterFrameWidth(qreal width) {
	Q_ASSERT(width >= 0);

	if( m_outerFrameWidth != width) {
		m_outerFrameWidth = width;
		m_adjusted = false;
		emit changed();
	}
}

void Table::setInnerFrameWidth(qreal width) {
	Q_ASSERT(width >= 0);

	if( m_innerFrameWidth != width) {
		m_innerFrameWidth = width;
		m_adjusted = false;
		for( unsigned int r=0; r<m_rows; ++r){
			for( unsigned int c=0; c<m_cols; ++c) {
				m_cells[c][r].setBorderWidth(TableCell::WholeFrame, width);
			}
		}
		emit changed();
	}
}

QFont Table::defaultFont() const {
	return m_textDocument->defaultFont();
}

void Table::setDefaultFont(const QFont& font) {
	m_textDocument->setDefaultFont(font);
	m_adjusted = false;
	emit changed();
}

void Table::setDefaultStyleSheet ( const QString & sheet ) {
	QString oldSheet = m_textDocument->defaultStyleSheet();
	if(oldSheet == sheet)
		return;

	m_textDocument->setDefaultStyleSheet(sheet);
	m_adjusted = false;
	emit changed();
}

void Table::repaint() {
	emit changed();
}

// private helper functions for adjust

qreal Table::cellWidthSpace(const TableCell& cell) const {
	return (2 * m_spacing + cell.leftMargin() + cell.rightMargin()) * m_scale;
}

qreal Table::cellHeightSpace(const TableCell& cell) const {
	return (2 * m_spacing + cell.topMargin() + cell.bottomMargin()) * m_scale;
}

void Table::setPaintProperties() {
	for( unsigned int c=0; c<m_cols; ++c) {
		// set text properties from text document
		for( unsigned int r=0; r<m_rows; ++r) {
			m_cells[c][r].setPaintProperties(m_textDocument);
		}
	}
}

void Table::calcMaxCellWidthsUnmerged(QVector<qreal>& maxCellWidthsUnmerged) {
	maxCellWidthsUnmerged.resize((int)m_cols);
	for( unsigned int c=0; c<m_cols; ++c) {
		// get column width
		qreal tmw(0);
		// for fixed column width
		if( m_columnWidths[(int)c].m_format == CellSizeFormater::Fixed) {
			tmw = m_columnWidths[(int)c].m_fixedSize;
		}
		// for automatic look for maximum cell width in the current column
		else {
			for( unsigned int r=0; r<m_rows; ++r) {
				TableCell& cell = m_cells[c][r];
				if( !cell.merged()) {
					QSizeF textSize = cell.textSize(-1);
					tmw = std::max(tmw, textSize.width());
				}
			}
		}
		// look for maximum sum of left and right margins in current column
		qreal maxMarginSum = 0;
		for( unsigned int r=0; r<m_rows; ++r) {
			const TableCell& cell = m_cells[c][r];
			if( !cell.merged()) {
				maxMarginSum = std::max(maxMarginSum, qreal(cell.leftMargin() + cell.rightMargin()));
			}
		}
		maxCellWidthsUnmerged[(int)c] = tmw + (2 * m_spacing + maxMarginSum) * m_scale;
	}
}

void Table::calcCellRectsMergedFromText(QVector<qreal>& maxCellWidthsUnmerged) {
	// Take text rectangles for merged cells and
	// adjust column widths in order to fit the text in merged cells
	for( int i=0, count = m_mergedCells.size(); i<count; ++i) {
		MergedCells& mc = m_mergedCells[i];
		qreal mw(0.0);
		// calc total width for all columns in range
		for( unsigned int c=mc.firstCol; c<=mc.lastCol;++c)
			mw += maxCellWidthsUnmerged[(int)c];

		TableCell& leftTopCell = m_cells[mc.firstCol][mc.firstRow];
		TableCell& rightTopCell = m_cells[mc.lastCol][mc.firstRow];
		qreal widthMarginSum = leftTopCell.leftMargin() + rightTopCell.rightMargin();

		QSizeF textSize = leftTopCell.textSize(-1);

		// fits the current text into the merged cell range?
		// calc difference between necessary space for text and current cell width
		qreal wdiff = mw - (2.0 * m_spacing + widthMarginSum) * m_scale - textSize.width();
		// make all columns in range bigger in order to create enough space
		if( wdiff < 0) {
			qreal additional = wdiff / (mc.lastCol - mc.firstCol + 1) * -1.0;
			mw = 0.0;
			for( unsigned int c=mc.firstCol; c<=mc.lastCol;++c) {
				maxCellWidthsUnmerged[(int)c] += additional;
				mw += maxCellWidthsUnmerged[(int)c];
			}
		}
		// save width of merge area (rect not valid)
		mc.m_cellRect = QRectF(0,0,mw,textSize.height());
	}
}

void Table::calcColumnWidthsExpanding(qreal availableTotalWidth, qreal currentTotalWidth, const QVector<qreal>& maxCellWidthsUnmerged) {
	// we have enough space
	// increase column widths in order to fit total size
	qreal restWidth = availableTotalWidth - currentTotalWidth;
	for( int c=0; c<(int)m_cols; ++c) {
		switch(m_columnWidths[c].m_format) {
			case CellSizeFormater::AutoMinimum: {
				m_columnWidths[c].m_size = maxCellWidthsUnmerged[c];
				break;
			}
			case CellSizeFormater::Fixed: {
				m_columnWidths[c].m_size = m_columnWidths[c].m_fixedSize;
				break;
			}
			case CellSizeFormater::TableWidth:
			case CellSizeFormater::Unknown: {
				m_columnWidths[c].m_size = maxCellWidthsUnmerged[c] + restWidth / m_cols;
				break;
			}
		}
	}
}

void Table::calcColumnWidthsShrinking(qreal availableTotalWidth, qreal currentTotalWidth, const QVector<qreal>& maxCellWidthsUnmerged) {
	// we have not enough space
	// shrink some column widths in order to fit total size
	/// \todo Doesnt work correctly, new implementation needed
	qreal missingSpace = currentTotalWidth - availableTotalWidth;

	// create temporary cell width vector for sorting
	QVector<qreal> tmaxWidths(maxCellWidthsUnmerged);
	std::sort(tmaxWidths.begin(), tmaxWidths.end());
	std::reverse(tmaxWidths.begin(), tmaxWidths.end());

	qreal save(0);
	qreal maxwidth(availableTotalWidth / m_cols);

	for( unsigned int c=1; c<m_cols; ++c) {
		save += tmaxWidths[0] - tmaxWidths[(int)c];
		if( save >= missingSpace) {
			qreal restsum = std::accumulate(tmaxWidths.begin() + c, tmaxWidths.end(), 0.0);
			maxwidth = (availableTotalWidth - restsum) / c;
			break;
		}
	}

	for( int c=0; c<(int)m_cols; ++c) {
		if( maxCellWidthsUnmerged[c] > maxwidth)
			m_columnWidths[c].m_size = maxwidth;
		else
			m_columnWidths[c].m_size = maxCellWidthsUnmerged[c];
	}
}

void Table::calcCellRectsWidthMergedFromColWidth() {
	// new calculation of cell widths for merged areas
	for( int i=0, count = m_mergedCells.size(); i<count; ++i) {
		MergedCells& mc = m_mergedCells[i];
		qreal mw(0.0);
		for( unsigned int c=mc.firstCol; c<=mc.lastCol;++c)
			mw += m_columnWidths[(int)c].m_size;
		// save width of merge area (rect not valid)
		mc.m_cellRect = QRectF(0,0,mw,0);
	}
}

void Table::calcMaxCellHeightsUnmerged(QVector<qreal>& maxCellHeightUnmerged) {
	maxCellHeightUnmerged.resize((int)m_rows);
	// Take text rectangles for all cells with width constraint and
	// creates a list of maximum cell height of each row
	for( unsigned int r=0; r<m_rows; ++r) {
		qreal tmh(0);
		qreal maxMarginSum = 0;
		for( unsigned int c=0; c<m_cols; ++c) {
			TableCell& cell = m_cells[c][r];
			if( !cell.merged()) {
				QSizeF textSize;
				if(cell.verticalText()) {
					textSize = cell.textSize(-1);
				}
				else {
					textSize = cell.textSize(m_columnWidths[(int)c].m_size - cellWidthSpace(cell));
				}
				tmh = std::max(tmh, textSize.height());
				maxMarginSum = std::max(maxMarginSum, cell.topMargin() + cell.bottomMargin());
			}
		}
		maxCellHeightUnmerged[(int)r] = tmh + (2 * m_spacing + maxMarginSum) * m_scale;
	}
}

void Table::calcCellRectsHeightMergedFromRowHeights(QVector<qreal>& maxCellHeightUnmerged) {
	for( int i=0, count = m_mergedCells.size(); i<count; ++i) {
		MergedCells& mc = m_mergedCells[i];
		TableCell& cell = m_cells[mc.firstCol][mc.firstRow];
		qreal mh(0.0);
		for( unsigned int r=mc.firstRow; r<=mc.lastRow;++r)
			mh += maxCellHeightUnmerged[(int)r];
		QSizeF textSize;
		if(cell.verticalText()) {
			textSize = cell.textSize(-1);
		}
		else {
			textSize = cell.textSize(mc.m_cellRect.width());
		}
		qreal textHeight = textSize.height();
		qreal topMargin = cell.topMargin();
		qreal bottomMargin = cell.bottomMargin();

		qreal hdiff = mh - (2* m_spacing +topMargin + bottomMargin) * m_scale - textHeight;
		if( hdiff < 0) {
			qreal additional = hdiff / (mc.lastRow - mc.firstRow + 1) * -1.0;
			mh = 0.0;
			for( unsigned int r=mc.firstRow; r<=mc.lastRow;++r) {
				maxCellHeightUnmerged[(int)r] += additional;
				mh += maxCellHeightUnmerged[(int)r];
			}
		}
		// save height of merge area (rect not valid because position wrong)
		mc.m_cellRect = QRectF(0,0,mc.m_cellRect.width(), mh);
	}
}

void Table::calcRowHeightsExpanding(qreal availableTotalHeight, qreal currentTotalHeight, const QVector<qreal>& maxCellHeightUnmerged) {
	// compare minimum table height with current table height
	// adjust table height if necessary
	qreal restHeight = availableTotalHeight - currentTotalHeight;
	if( restHeight < 0)
		restHeight = 0;
	// we have enough space
	// increase column widths in order to fit total size
	for( int r=0; r<(int)m_rows; ++r) {
		switch(m_rowHeights[r].m_format) {
			case CellSizeFormater::AutoMinimum: {
				m_rowHeights[r].m_size = maxCellHeightUnmerged[r];
				break;
			}
			case CellSizeFormater::Fixed: {
				m_rowHeights[r].m_size = m_rowHeights[r].m_fixedSize;
				break;
			}
			case CellSizeFormater::TableWidth:
			case CellSizeFormater::Unknown: {
				m_rowHeights[r].m_size = maxCellHeightUnmerged[r] + restHeight / m_rows;
			}
		}
	}
}

void Table::setCellRects() {
	// Create rects for single cells without consideration of merging
	qreal xpos(m_margin * m_scale);
	for( unsigned int c=0; c<m_cols; ++c) {
		qreal ypos(m_margin * m_scale);
		for( unsigned int r=0; r<m_rows; ++r) {
			QRectF crect = QRectF(xpos, ypos, m_columnWidths[(int)c].m_size, m_rowHeights[(int)r].m_size);
			m_cells[c][r].setCellRect(crect);
			ypos +=  m_rowHeights[(int)r].m_size;
		}
		xpos += m_columnWidths[(int)c].m_size;
	}
}

void  Table::setCellMaximumTextRects() {
	for( unsigned int c=0; c<m_cols; ++c) {
		for( unsigned int r=0; r<m_rows; ++r) {
			TableCell& currentCell = m_cells[c][r];
			const QRectF& crect = currentCell.cellRect();
			qreal mtleft = crect.x() + (m_spacing + currentCell.leftMargin()) * m_scale;
			qreal mtwidth = crect.width() - (2 * m_spacing + currentCell.leftMargin() + currentCell.rightMargin()) * m_scale;
			qreal mttop = crect.y() + (m_spacing + currentCell.topMargin()) * m_scale;
			qreal mtheight = crect.height() - (2 * m_spacing + currentCell.topMargin() + currentCell.bottomMargin()) * m_scale;
			QRectF maxTextRect = QRectF( mtleft, mttop, mtwidth, mtheight);
			if(currentCell.verticalText())
				maxTextRect = maxTextRect.transposed();
			currentCell.setMaxTextRect(maxTextRect);
		}
	}
}

void  Table::calcHorizontalLines() {
	for( unsigned int c=0; c<m_cols; ++c) {
		for( unsigned int r=0; r<m_rows; ++r) {
			TableCell& currentCell = m_cells[c][r];
			const QRectF& crect = currentCell.cellRect();

			// width for border lines
			qreal hw = currentCell.borderWidth(TableCell::TopBorder);
			if( r > 0)
				hw = std::max(hw, m_cells[c][r - 1].borderWidth(TableCell::BottomBorder));

			int mindex = mergeIndex(c, r);
			if( mindex != -1) {
				const MergedCells& mc = m_mergedCells[mindex];
				if( r !=  mc.firstRow)
					hw = 0;
			}

			m_HLines[c][r] = LineProperties(QLineF(crect.topLeft(), crect.topRight()));
			m_HLines[c][r].m_width = hw;
		}
	}

	// bottom lines
	for( unsigned int c=0; c<m_cols; ++c) {
		const TableCell& currentCell = m_cells[c][m_rows-1];
		const QRectF& crect = currentCell.cellRect();
		m_HLines[c][m_rows] = LineProperties(QLineF(crect.bottomLeft(), crect.bottomRight()));
		m_HLines[c][m_rows].m_width = currentCell.borderWidth(TableCell::BottomBorder);
	}
}

void  Table::calcVerticalLines() {
	for( unsigned int c=0; c<m_cols; ++c) {
		for( unsigned int r=0; r<m_rows; ++r) {
			const TableCell& currentCell = m_cells[c][r];
			const QRectF& crect = currentCell.cellRect();

			m_VLines[c][r] = LineProperties(QLineF(crect.topLeft(), crect.bottomLeft()));

			// width for border lines
			qreal vw = currentCell.borderWidth(TableCell::LeftBorder);
			if( c > 0)
				vw = std::max(vw, m_cells[c - 1][r].borderWidth(TableCell::RightBorder));

			int mindex = mergeIndex(c, r);
			if( mindex != -1) {
				if( c !=  m_mergedCells[mindex].firstCol)
					vw = 0;
			}

			m_VLines[c][r].m_width = vw;
		}
	}

	// right border
	for( unsigned int r=0; r<m_rows; ++r) {
		const TableCell& currentCell = m_cells[m_cols-1][r];
		const QRectF& crect = currentCell.cellRect();
		m_VLines[m_cols][r] = LineProperties(QLineF(crect.topRight(), crect.bottomRight()));
		m_VLines[m_cols][r].m_width = currentCell.borderWidth(TableCell::RightBorder);
	}
}

void Table::setCellRectsMerged() {
	// Create rects for merge areas
	for( int i=0, count = m_mergedCells.size(); i<count; ++i) {
		MergedCells& mc = m_mergedCells[i];

		qreal width(0);
		for( unsigned int c=mc.firstCol; c<=mc.lastCol; ++c)
			width += m_columnWidths[c].m_size;

		qreal height(0);
		for( unsigned int r=mc.firstRow; r<=mc.lastRow; ++r)
			height += m_rowHeights[r].m_size;

		qreal left = m_cells[mc.firstCol][mc.firstRow].cellRect().left();
		qreal top = m_cells[mc.firstCol][mc.firstRow].cellRect().top();
		QRectF crect(left, top, width, height);
		mc.m_cellRect = crect;
	}
}

void Table::setCellMaximumTextRectsMerged() {
	// Create rects for merge areas
	for( int i=0, count = m_mergedCells.size(); i<count; ++i) {
		MergedCells& mc = m_mergedCells[i];
		const QRectF& crect = mc.m_cellRect;

		qreal leftMargin =  m_cells[mc.firstCol][mc.firstRow].leftMargin();
		qreal rightMargin = m_cells[mc.lastCol][mc.firstRow].rightMargin();
		qreal topMargin =  m_cells[mc.firstCol][mc.firstRow].topMargin();
		qreal bottomMargin = m_cells[mc.firstCol][mc.lastRow].bottomMargin();

		qreal mtleft = crect.x() + (m_spacing + leftMargin) * m_scale;
		qreal mtwidth = crect.width() - (2 * m_spacing + leftMargin + rightMargin) * m_scale;
		qreal mttop = crect.y() + m_spacing * m_scale;
		qreal mtheight = crect.height() - (2 * m_spacing + topMargin + bottomMargin) * m_scale;
		mc.m_maxTextRect = QRectF( mtleft, mttop, mtwidth, mtheight);
	}
}


void  Table::setCellTextRects() {
	for( unsigned int c=0; c<m_cols; ++c) {
		for( unsigned int r=0; r<m_rows; ++r) {
			TableCell& currentCell = m_cells[c][r];
			if( !currentCell.merged()) {
				const QRectF& maxTextRect = currentCell.maxTextRect();

				QSizeF textSize;
				if(currentCell.verticalText())
					textSize = currentCell.textSize(maxTextRect.height());
				else
					textSize = currentCell.textSize(maxTextRect.width());
				QRectF trect(maxTextRect.topLeft(), textSize);
//				if(currentCell.verticalText())
//					trect = trect.transposed();
				currentCell.setTextRect(trect);
			}
		}
	}
}

void Table::setCellTextRectsMerged() {
	// Create text rects for merge areas
	for( int i=0, count = m_mergedCells.size(); i<count; ++i) {
		MergedCells& mc = m_mergedCells[i];
		const QRectF& maxTextRect = mc.m_maxTextRect;

		QSizeF textSize;
		TableCell& leftTopCell = m_cells[mc.firstCol][mc.firstRow];
		if( leftTopCell.verticalText())
			textSize = m_cells[mc.firstCol][mc.firstRow].textSize(maxTextRect.height());
		else
			textSize = m_cells[mc.firstCol][mc.firstRow].textSize(maxTextRect.width());
		QRectF trect(maxTextRect.topLeft(), textSize);

		mc.m_textRect = trect;
	}
}


void Table::adjust(QPaintDevice* paintDevice) {
	// performance feature - if nothing is changed no adjustment necessary
	if( m_adjusted)
		return;

	m_textDocument->documentLayout()->setPaintDevice(paintDevice);
	m_scale = paintDevice->logicalDpiX() / QApplication::desktop()->screen()->logicalDpiX();

	setPaintProperties();

	// Take text rectangles for all cells without width constraint and
	// creates a list of maximum cell width of each column
	QVector<qreal> maxCellWidthsUnmerged;
	calcMaxCellWidthsUnmerged(maxCellWidthsUnmerged);

	// Take text rectangles for merged cells and
	// adjust column widths in order to fit the text in merged cells
	calcCellRectsMergedFromText(maxCellWidthsUnmerged);

	// calculates the minimum table width
	qreal tablewidth = m_size.width() - 2 * m_margin * m_scale;
	// check if size is set correctly
	Q_ASSERT(tablewidth > 0);

	qreal mwTotal = std::accumulate(maxCellWidthsUnmerged.begin(), maxCellWidthsUnmerged.end(), 0.0);

	// compare minimum table width with current table width
	// adjust column widths if necessary
	if( tablewidth >= mwTotal) {
		// we have enough space
		// increase column widths in order to fit total size
		calcColumnWidthsExpanding(tablewidth, mwTotal, maxCellWidthsUnmerged);
	}
	else {
		// we have not enough space
		// shrink some column widths in order to fit total size
		calcColumnWidthsShrinking(tablewidth, mwTotal, maxCellWidthsUnmerged);
	}

	// new calculation of cell widths for merged areas
	calcCellRectsWidthMergedFromColWidth();

	// Take text rectangles for all cells with width constraint and
	// creates a list of maximum cell height of each row
	QVector<qreal> maxCellHeightUnmerged;
	calcMaxCellHeightsUnmerged(maxCellHeightUnmerged);

	// Take text rectangles for merged cells and
	// adjust row heights in order to fit the text in merged cells
	calcCellRectsHeightMergedFromRowHeights(maxCellHeightUnmerged);

	// calculates the minimum table height
	qreal tableheight = m_size.height() - 2 * m_margin * m_scale;
	if(tableheight < 0) {
		// means we have more than enough height
		tableheight = 0;
	}

	qreal mhTotal(0.0);
	for( unsigned int r=0; r<m_rows; ++r)
		mhTotal += maxCellHeightUnmerged[r];


	// compare minimum table height with current table height
	// adjust table height if necessary
	// tableheight == 0 means no minimum exist

	// we have enough space
	///< \todo implement method in case of tables higher than page height
	calcRowHeightsExpanding(tableheight, mhTotal, maxCellHeightUnmerged);

	// calculate current total table height
	mhTotal = 0.0;
	for( unsigned int r=0; r<m_rows; ++r) {
		mhTotal += m_rowHeights[r].m_size;
	}
	m_size.setHeight(mhTotal + 2 * m_margin * m_scale);

	// set all cell rects
	setCellRects();

	// set text rect inseide cell rect
	setCellMaximumTextRects();

	calcHorizontalLines();

	calcVerticalLines();

	// Create rects for merge areas
	setCellRectsMerged();
	setCellMaximumTextRectsMerged();

	// create text rects for non merged cells
	setCellTextRects();
	setCellTextRectsMerged();

	m_adjusted = true;
}

QRectF Table::textRect(const QString& html, qreal width) const {
	// No text - no rect necessary
	if( html.isEmpty())
		return QRectF();

	m_textDocument->setTextWidth(width);
	m_textDocument->setDocumentMargin(0);
	m_textDocument->setHtml(html);
	qreal maxw = m_textDocument->idealWidth();		// forces creating of layout and text lines
	QSizeF ds = m_textDocument->documentLayout()->documentSize();
	QRectF rect(0, 0, maxw, ds.height());
	return rect;
}

void Table::paintCellText(unsigned int col, unsigned int row, QPainter* painter, const QPointF& pos) {
	Q_ASSERT(col < m_cols);
	Q_ASSERT(row < m_rows);

	// takes cell and text rect
	const TableCell& currentCell = m_cells[col][row];
	QString text = currentCell.text();
	// No text - nothing to paint
	if( text.isEmpty())
		return;

	QRectF cellRect = currentCell.cellRect();
	QRectF textRect =  currentCell.textRect();
	QRectF maxTextRect =  currentCell.maxTextRect();
	int mindex = mergeIndex(col, row);
	// if cell is part of merging area take cell and text rect from this
	if( mindex != -1) {
		MergedCells& mc = m_mergedCells[mindex];
		cellRect = mc.m_cellRect;
		textRect = mc.m_textRect;
		maxTextRect =  mc.m_maxTextRect;
	}

	// calculate rect and positions
	qreal xpos(maxTextRect.left());
	qreal ypos(maxTextRect.top());
	Qt::Alignment align = currentCell.alignment();
	if( align.testFlag(Qt::AlignHCenter) || align.testFlag(Qt::AlignCenter))
		xpos = maxTextRect.left() + (maxTextRect.width() - textRect.width()) / 2.0;
	if( align.testFlag(Qt::AlignRight))
		xpos = maxTextRect.right() - textRect.width();
	if( align.testFlag(Qt::AlignBottom))
		ypos = maxTextRect.bottom() - textRect.height();
	if( align.testFlag(Qt::AlignVCenter))
		ypos = (maxTextRect.height() - textRect.height()) / 2.0 + maxTextRect.top();
	if(currentCell.verticalText()) {
		ypos += textRect.height();
	}

	// calculation of offset
	qreal voffset = currentCell.verticalOffset() * textRect.height();
	ypos += voffset;

	// set document properties
	if(currentCell.verticalText())
		m_textDocument->setTextWidth(textRect.height());
	else
		m_textDocument->setTextWidth(textRect.width());
	m_textDocument->setDocumentMargin(0);
	text = "<meta charset=\"utf-8\"/>" + text;
	m_textDocument->setHtml(text);
//	m_textDocument->setDefaultStyleSheet("h1 {font-family:Times; font-weight:normal; font-size: large;}");
	m_textDocument->idealWidth();		// forces creating of layout and text lines

	painter->save();
	painter->translate(QPointF(xpos, ypos) + pos);
	if(currentCell.verticalText())
		painter->rotate(-90);
	QTextOption option = m_textDocument->defaultTextOption();
	QTextOption optionOrg = option;
	option.setAlignment(align);
	m_textDocument->setDefaultTextOption(option);
	m_textDocument->drawContents(painter);
	m_textDocument->setDefaultTextOption(optionOrg);
	painter->restore();
}

void Table::paintCellRects(QPainter* painter, const QPointF& pos) {
	// set lines and pen
	painter->save();
	for( unsigned int c=0; c<m_cols; ++c) {
		for( unsigned int r=0; r<m_rows; ++r) {
			const TableCell& currentCell = m_cells[c][r];
			// paint cell background
			painter->fillRect(currentCell.cellRect().translated(pos), currentCell.backgroundColor());
		}
	}

	QPen pen = painter->pen();
	pen.setJoinStyle(Qt::MiterJoin);

	// draw horizontal lines
	for( int c=0; c<m_HLines.size(); ++c) {
		for( int r=0; r<m_HLines[c].size(); ++r) {
			qreal w = m_HLines[c][r].m_width * m_scale;
			if( w > 0) {
				pen.setWidthF(w);
				painter->setPen(pen);
				painter->drawLine(m_HLines[c][r].m_line.translated(pos));
			}
		}
	}

	// draw vertical lines
	for( int c=0; c<m_VLines.size(); ++c) {
		for( int r=0; r<m_VLines[c].size(); ++r) {
			qreal w = m_VLines[c][r].m_width * m_scale;
			if( w > 0) {
				pen.setWidthF(w);
				painter->setPen(pen);
				painter->drawLine(m_VLines[c][r].m_line.translated(pos));
			}
		}
	}
	painter->restore();
}

QRectF Table::tableRect(QPaintDevice* paintDevice, qreal width) {
	QRectF result;
	if( !paintDevice)
		return result;

	if( width <= 0)
		width = paintDevice->width();
	m_size = QSize((int)width, -1);

	if( !m_adjusted) {
		adjust(paintDevice);
	}
	qreal localeWidth = m_VLines.back().front().m_line.x1() - m_VLines.front().front().m_line.x1();
	qreal localeHeight = m_VLines.front().back().m_line.y2() - m_VLines.front().front().m_line.y1();
	result.setSize(QSizeF(localeWidth, localeHeight));
	return result;
}

QSizeF Table::tableSize() const {
	qreal localeWidth = m_VLines.back().front().m_line.x1() - m_VLines.front().front().m_line.x1();
	qreal localeHeight = m_VLines.front().back().m_line.y2() - m_VLines.front().front().m_line.y1();
	return QSizeF(localeWidth, localeHeight);
}

void Table::drawTable(QPainter* painter, const QPointF& pos) {
	// makes no sense to draw an empty table
	if( m_rows <= 0 || m_cols <= 0)
		return;

	Q_ASSERT(painter != NULL);
	QPaintDevice* device = painter->device();
	Q_ASSERT(device != NULL);

	painter->fillRect(QRectF(pos, m_size), m_background);

	if( m_size.width() <= 0)
		m_size = QSize(device->width(), -1);


	adjust(device);
	paintCellRects(painter, pos);
	for( unsigned int j=0; j<m_rows; ++j){
		for( unsigned int i=0; i<m_cols; ++i) {
			paintCellText(i, j, painter, pos);
		}
	}
	if( m_outerFrameWidth > 0) {
		QPen orgpen = painter->pen();
		QPen pen = painter->pen();
		pen.setWidthF(m_outerFrameWidth * m_scale);
		painter->setPen(pen);
		QPointF topleft = m_cells[0][0].cellRect().topLeft();
		QPointF bottomright = m_cells[m_cols - 1][m_rows - 1].cellRect().bottomRight();
		painter->drawRect(QRectF(topleft, bottomright).translated(pos));
		painter->setPen(orgpen);
	}
}

void Table::frameRect(Table& table, int cLeft, int cRight, int rTop, int rBottom, int lineWidth) {
	for( int i=rTop; i<=rBottom; ++i) {
		table.cell(cLeft, i).setBorderWidth(QtExt::TableCell::LeftBorder, lineWidth);
		table.cell(cRight, i).setBorderWidth(QtExt::TableCell::RightBorder, lineWidth);
	}
	for( int i=cLeft; i<=cRight; ++i) {
		table.cell(i, rTop).setBorderWidth(QtExt::TableCell::TopBorder, lineWidth);
		table.cell(i, rBottom).setBorderWidth(QtExt::TableCell::BottomBorder, lineWidth);
	}
}


} // namespace QtExt
