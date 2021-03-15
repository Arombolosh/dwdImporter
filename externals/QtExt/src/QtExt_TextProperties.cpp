/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_TextProperties.h"

#include <memory>

#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>

namespace QtExt {

TextProperties::TextProperties(const QString& fontFamily, int fontPointSize,
							   QTextDocument* textDocument, QPaintDevice* paintDevice) :
	m_normalFont(fontFamily, fontPointSize)
{
	m_normalFont.setKerning(false);
	update(textDocument, paintDevice);
}

void TextProperties::update(QTextDocument* textDocument, QPaintDevice* paintDevice) {

	Q_ASSERT(textDocument);
	std::unique_ptr<QTextDocument> m_textDocument(textDocument->clone());
	if( paintDevice)
		m_textDocument->documentLayout()->setPaintDevice(paintDevice);
	m_textDocument->setDefaultFont(m_normalFont);

	QString testString = QString::fromWCharArray(L"QWERTZUIOPÃASDFGHJKLÃÃYXCVBNM;:_qwertzuiopÃ¼asdfghjklÃ¶Ã¤yxcvbnm,.-");

	m_textDocument->setHtml(testString);
	m_textDocument->idealWidth();		// forces creating of layout and text lines

	QTextBlock block = m_textDocument->firstBlock();
	QTextLayout* layout = block.layout();
	m_textNormal.height = layout->lineAt(0).height();
	m_textNormal.ascent = layout->lineAt(0).ascent();
	m_textNormal.descent = layout->lineAt(0).descent();

	m_textDocument->setHtml(QString("<h1>%1</h1>").arg(testString));
	m_textDocument->idealWidth();		// forces creating of layout and text lines
	block = m_textDocument->firstBlock();
	layout = block.layout();
	m_textH1.height = layout->lineAt(0).height();
	m_textH1.ascent = layout->lineAt(0).ascent();
	m_textH1.descent = layout->lineAt(0).descent();

	m_textDocument->setHtml(QString("<h2>%1</h2>").arg(testString));
	m_textDocument->idealWidth();		// forces creating of layout and text lines
	block = m_textDocument->firstBlock();
	layout = block.layout();
	m_textH2.height = layout->lineAt(0).height();
	m_textH2.ascent = layout->lineAt(0).ascent();
	m_textH2.descent = layout->lineAt(0).descent();

	m_textDocument->setHtml(QString("<h3>%1</h3>").arg(testString));
	m_textDocument->idealWidth();		// forces creating of layout and text lines
	block = m_textDocument->firstBlock();
	layout = block.layout();
	m_textH3.height = layout->lineAt(0).height();
	m_textH3.ascent = layout->lineAt(0).ascent();
	m_textH3.descent = layout->lineAt(0).descent();

	m_textDocument->setHtml(QString("<h4>%1</h4>").arg(testString));
	m_textDocument->idealWidth();		// forces creating of layout and text lines
	block = m_textDocument->firstBlock();
	layout = block.layout();
	m_textH4.height = layout->lineAt(0).height();
	m_textH4.ascent = layout->lineAt(0).ascent();
	m_textH4.descent = layout->lineAt(0).descent();

	m_textDocument->setHtml(QString("<small>%1</small>").arg(testString));
	m_textDocument->idealWidth();		// forces creating of layout and text lines
	block = m_textDocument->firstBlock();
	layout = block.layout();
	m_textSmall.height = layout->lineAt(0).height();
	m_textSmall.ascent = layout->lineAt(0).ascent();
	m_textSmall.descent = layout->lineAt(0).descent();
}

} // namespace QtExt {
