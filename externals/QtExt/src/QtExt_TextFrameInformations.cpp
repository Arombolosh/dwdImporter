/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include <memory>
#include <algorithm>

#include <QTextDocument>
#include <QTextLayout>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>
#include <QFontMetrics>
#include <QDebug>

#include "QtExt_TextFrameInformations.h"
#include "QtExt_TextFrame.h"

namespace QtExt {

TextFrameInformations::TextFrameInformations() :
		m_textDocument(0)
{
}

TextFrameInformations::TextFrameInformations(QTextDocument* textDocument, const QString& text) :
		m_textDocument(0)
{
	set(textDocument, text);
}

TextFrameInformations::~TextFrameInformations() {
//	delete m_textDocument;
}

TextFrameInformations::TextFrameInformations(const TextFrameInformations& src)  :
	m_textDocument(src.m_textDocument),
	m_infoVect(src.m_infoVect),
	m_text(src.m_text)
{
}

TextFrameInformations& TextFrameInformations::operator=(TextFrameInformations src) {
	Swap(src);
	return *this;
}

void TextFrameInformations::Swap(TextFrameInformations& src) {
	std::swap(m_textDocument, src.m_textDocument);
	m_infoVect.swap(src.m_infoVect);
	std::swap(m_text, src.m_text);
}

// return current total text size
// text and width must be set before
QSizeF totalTextSize(QTextDocument* textDocument) {
	QTextBlock block = textDocument->begin();
	QRectF totalBoundingRect;
	while(block.isValid()) {
		// create rectangle that can contain rectangles of all blocks
		QRectF rect = textDocument->documentLayout()->blockBoundingRect(block);
		totalBoundingRect = totalBoundingRect.united(rect);
		block = block.next();
	}
	return totalBoundingRect.size();
}

// check if document line count equals internal block line count
// if not lyout doesn't fulfill requirements for using TextFrameInformations class
bool checkValidLayout(QTextDocument* textDocument) {
	textDocument->idealWidth();
	int blocklLineCount = 0;
	QTextBlock block = textDocument->begin();
	while(block.isValid()) {
		int blc = block.lineCount();
		// if QTextBlock doesn't support this function dont't check this
		if(blc == -1)
			return true;

		blocklLineCount += block.lineCount();
		block = block.next();
	}
	return textDocument->lineCount() == blocklLineCount;
}


void TextFrameInformations::set(QTextDocument* textDocument, const QString& text) {
	m_textDocument = textDocument;
	Q_ASSERT(m_textDocument != 0);
	if( text != m_text) {
		m_infoVect.clear();
		m_text = text;
	}
	else {
		return;
	}

	// No text - no rect needed
	if( text.isEmpty()) {
		return;
	}

	m_textDocument->setHtml(m_text);
	m_textDocument->setDocumentMargin(0);

	// start with infinite width
	m_textDocument->setTextWidth(-1);
	qreal iwidth = m_textDocument->idealWidth();		// forces creating of layout and text lines
	QSizeF textSize = totalTextSize(m_textDocument);
	int linecount = m_textDocument->lineCount();
	m_infoVect.push_back(TextFrameInfo(linecount, textSize.height(), textSize.width(), 1e10));

	// now look for other widths
	qreal currentHeight = textSize.height();
	int i = textSize.width();
	while( i>1) {
		m_textDocument->setTextWidth(i - 1);
		qreal lastiwidth = iwidth;
		iwidth = m_textDocument->idealWidth();		// forces creating of layout and text lines
		textSize = totalTextSize(m_textDocument);
		if(currentHeight < textSize.height()) {
			++linecount;
			m_infoVect.push_back(TextFrameInfo(linecount, textSize.height(), textSize.width(), m_infoVect.back().m_minWidth - 1));
			currentHeight = textSize.height();
		}
		else {
			m_infoVect.back().m_minWidth = textSize.width();
		}
		if(iwidth < lastiwidth) {
			i = iwidth;
		}
		else {
			--i;
		}
	}
}

void TextFrameInformations::setInternal() {
	set(m_textDocument, m_text);
}


TextFrameInformations::TextFrameInfo TextFrameInformations::sizeForMaximumWidth(qreal maxWidth) {
	if( m_text.isEmpty())
		return TextFrameInformations::TextFrameInfo::nonValid(0);

	// create internal vector if not already done
	// first only for maximum width
	if( m_infoVect.empty())
		setInternal();

	// if internal vector is already empty somthing serious is wrong
	if( m_infoVect.empty())
		return TextFrameInformations::TextFrameInfo::nonValid(0);

	if(maxWidth < 0) {
		Q_ASSERT(m_infoVect.front().isValid());
		return m_infoVect.front();
	}

	// look for existing size object which can be drawn into given width
	for( size_t i=0, count=m_infoVect.size(); i<count; ++i) {
		const TextFrameInfo& info = m_infoVect[i];
		if(info.isValid()) {

			if(info.m_maxWidth > maxWidth && info.m_minWidth <= maxWidth)
				return info;

		}
	}

	return TextFrameInformations::TextFrameInfo::nonValid(0);
}

} // namespace QtExt
