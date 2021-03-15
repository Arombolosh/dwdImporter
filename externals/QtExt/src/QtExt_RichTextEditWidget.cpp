/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

// Original source code from Qt5 examples

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QMessageBox>
#include <QMimeData>
#include <QVBoxLayout>

#include "QtExt_RichTextEditWidget.h"

const QString rsrcPath = ":/master";

namespace QtExt {

RichTextEditWidget::RichTextEditWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle(QCoreApplication::applicationName());

	textEdit = new SignalingTextEdit(this);
	connect(textEdit, &QTextEdit::currentCharFormatChanged,
			this, &RichTextEditWidget::currentCharFormatChanged);
	connect(textEdit, &QTextEdit::cursorPositionChanged,
			this, &RichTextEditWidget::cursorPositionChanged);
	QVBoxLayout * lay = new QVBoxLayout();
	lay->addWidget(textEdit);
	lay->setMargin(0);
	setLayout(lay);

	setupEditActions();
	setupTextActions();

#ifdef Q_OS_WIN
	QFont textFont("Helvetica");
#elif defined(Q_OS_LINUX)
	QFont textFont("Ubuntu");
#else
	QFont textFont("Helvetica");
#endif
	textFont.setStyleHint(QFont::SansSerif);
	textEdit->setFont(textFont);
	fontChanged(textEdit->font());
	colorChanged(textEdit->textColor());
	alignmentChanged(textEdit->alignment());

	connect(textEdit->document(), &QTextDocument::undoAvailable,
			actionUndo, &QAction::setEnabled);
	connect(textEdit->document(), &QTextDocument::redoAvailable,
			actionRedo, &QAction::setEnabled);

	actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
	actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

#ifndef QT_NO_CLIPBOARD
	actionCut->setEnabled(false);
	actionCopy->setEnabled(false);

	connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &RichTextEditWidget::clipboardDataChanged);
#endif

	textEdit->setFocus();

	connect(textEdit, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
}


QString RichTextEditWidget::htmlText() const {
	QString html = textEdit->toHtml();
	int pos = html.indexOf("<html>");
	html = html.right(html.length() - pos);
	// extract everything within the body tag
	pos = html.indexOf("<body");
	pos = html.indexOf(">", pos);
	int pos2 = html.lastIndexOf("</body>");
	html = html.mid(pos+1, pos2 - pos-1);

	/// \todo Do some intelligent substitution, detect headings and regular paragraphs

	return html;
}


void RichTextEditWidget::setupEditActions() {
	QToolBar *tb = addToolBar(tr("Edit Actions"));

	const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
	actionUndo = tb->addAction(undoIcon, tr("&Undo"), this, SLOT(undo()));
	actionUndo->setShortcut(QKeySequence::Undo);

	const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
	actionRedo = tb->addAction(redoIcon, tr("&Redo"), this, SLOT(redo()));
	actionRedo->setPriority(QAction::LowPriority);
	actionRedo->setShortcut(QKeySequence::Redo);

#ifndef QT_NO_CLIPBOARD
	const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
	actionCut = tb->addAction(cutIcon, tr("Cu&t"), this, SLOT(cut()));
	actionCut->setPriority(QAction::LowPriority);
	actionCut->setShortcut(QKeySequence::Cut);

	const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
	actionCopy = tb->addAction(copyIcon, tr("&Copy"), this, SLOT(copy()));
	actionCopy->setPriority(QAction::LowPriority);
	actionCopy->setShortcut(QKeySequence::Copy);

	const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
	actionPaste = tb->addAction(pasteIcon, tr("&Paste"), this, SLOT(paste()));
	actionPaste->setPriority(QAction::LowPriority);
	actionPaste->setShortcut(QKeySequence::Paste);
	if (const QMimeData *md = QApplication::clipboard()->mimeData())
		actionPaste->setEnabled(md->hasText());
#endif
}

void RichTextEditWidget::setupTextActions()
{
	QToolBar *tb = addToolBar(tr("Format Actions"));

	const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));
	actionTextBold = tb->addAction(boldIcon, tr("&Bold"), this, SLOT(textBold()));
	actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
	actionTextBold->setPriority(QAction::LowPriority);
	QFont bold;
	bold.setBold(true);
	actionTextBold->setFont(bold);
	actionTextBold->setCheckable(true);

	const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));
	actionTextItalic = tb->addAction(italicIcon, tr("&Italic"), this, SLOT(textItalic()));
	actionTextItalic->setPriority(QAction::LowPriority);
	actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
	QFont italic;
	italic.setItalic(true);
	actionTextItalic->setFont(italic);
	actionTextItalic->setCheckable(true);

	const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));
	actionTextUnderline = tb->addAction(underlineIcon, tr("&Underline"), this, SLOT(textUnderline()));
	actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
	actionTextUnderline->setPriority(QAction::LowPriority);
	QFont underline;
	underline.setUnderline(true);
	actionTextUnderline->setFont(underline);
	actionTextUnderline->setCheckable(true);

	const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
	actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
	actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
	actionAlignLeft->setCheckable(true);
	actionAlignLeft->setPriority(QAction::LowPriority);
	const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
	actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
	actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
	actionAlignCenter->setCheckable(true);
	actionAlignCenter->setPriority(QAction::LowPriority);
	const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
	actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
	actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
	actionAlignRight->setCheckable(true);
	actionAlignRight->setPriority(QAction::LowPriority);
	const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));
	actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);
	actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
	actionAlignJustify->setCheckable(true);
	actionAlignJustify->setPriority(QAction::LowPriority);

	// Make sure the alignLeft  is always left of the alignRight
	QActionGroup *alignGroup = new QActionGroup(this);
	connect(alignGroup, &QActionGroup::triggered, this, &RichTextEditWidget::textAlign);

	if (QApplication::isLeftToRight()) {
		alignGroup->addAction(actionAlignLeft);
		alignGroup->addAction(actionAlignCenter);
		alignGroup->addAction(actionAlignRight);
	} else {
		alignGroup->addAction(actionAlignRight);
		alignGroup->addAction(actionAlignCenter);
		alignGroup->addAction(actionAlignLeft);
	}
	alignGroup->addAction(actionAlignJustify);

	tb->addActions(alignGroup->actions());

	QPixmap pix(16, 16);
	pix.fill(Qt::black);
	actionTextColor = tb->addAction(pix, tr("&Color..."), this, SLOT(textColor()));

	tb = addToolBar(tr("Format Actions"));
	tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

	comboListFormat = new QComboBox(tb);
	tb->addWidget(comboListFormat);
	comboListFormat->addItem("No list");
	comboListFormat->addItem("Bullet List (Disc)");
	comboListFormat->addItem("Bullet List (Circle)");
	comboListFormat->addItem("Bullet List (Square)");
	comboListFormat->addItem("Ordered List (Decimal)");
	comboListFormat->addItem("Ordered List (Alpha lower)");
	comboListFormat->addItem("Ordered List (Alpha upper)");
	comboListFormat->addItem("Ordered List (Roman lower)");
	comboListFormat->addItem("Ordered List (Roman upper)");

	typedef void (QComboBox::*QComboIntSignal)(int);
	connect(comboListFormat, static_cast<QComboIntSignal>(&QComboBox::activated), this, &RichTextEditWidget::textStyle);

	typedef void (QComboBox::*QComboStringSignal)(const QString &);
	comboFont = new QFontComboBox(tb);
	tb->addWidget(comboFont);
	connect(comboFont, static_cast<QComboStringSignal>(&QComboBox::activated), this, &RichTextEditWidget::textFamily);

	comboSize = new QComboBox(tb);
	comboSize->setObjectName("comboSize");
	tb->addWidget(comboSize);
	comboSize->setEditable(true);

	const QList<int> standardSizes = QFontDatabase::standardSizes();
	foreach (int size, standardSizes)
		comboSize->addItem(QString::number(size));
	comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

	connect(comboSize, static_cast<QComboStringSignal>(&QComboBox::activated), this, &RichTextEditWidget::textSize);
}

void RichTextEditWidget::textBold() {
	QTextCharFormat fmt;
	fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditWidget::textUnderline()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(actionTextUnderline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditWidget::textItalic()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(actionTextItalic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditWidget::textFamily(const QString &f)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(f);
	mergeFormatOnWordOrSelection(fmt);
}

void RichTextEditWidget::textSize(const QString &p)
{
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void RichTextEditWidget::textStyle(int styleIndex)
{
	QTextCursor cursor = textEdit->textCursor();

	if (styleIndex != 0) {
		QTextListFormat::Style style = QTextListFormat::ListDisc;

		switch (styleIndex) {
			default:
			case 1:
				style = QTextListFormat::ListDisc;
				break;
			case 2:
				style = QTextListFormat::ListCircle;
				break;
			case 3:
				style = QTextListFormat::ListSquare;
				break;
			case 4:
				style = QTextListFormat::ListDecimal;
				break;
			case 5:
				style = QTextListFormat::ListLowerAlpha;
				break;
			case 6:
				style = QTextListFormat::ListUpperAlpha;
				break;
			case 7:
				style = QTextListFormat::ListLowerRoman;
				break;
			case 8:
				style = QTextListFormat::ListUpperRoman;
				break;
		}

		cursor.beginEditBlock();

		QTextBlockFormat blockFmt = cursor.blockFormat();

		QTextListFormat listFmt;

		if (cursor.currentList()) {
			listFmt = cursor.currentList()->format();
		} else {
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);
			cursor.setBlockFormat(blockFmt);
		}

		listFmt.setStyle(style);

		cursor.createList(listFmt);

		cursor.endEditBlock();
	} else {
		// ####
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}
}

void RichTextEditWidget::textColor()
{
	QColor col = QColorDialog::getColor(textEdit->textColor(), this);
	if (!col.isValid())
		return;
	QTextCharFormat fmt;
	fmt.setForeground(col);
	mergeFormatOnWordOrSelection(fmt);
	colorChanged(col);
}

void RichTextEditWidget::textAlign(QAction *a)
{
	if (a == actionAlignLeft)
		textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	else if (a == actionAlignCenter)
		textEdit->setAlignment(Qt::AlignHCenter);
	else if (a == actionAlignRight)
		textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	else if (a == actionAlignJustify)
		textEdit->setAlignment(Qt::AlignJustify);
}

void RichTextEditWidget::currentCharFormatChanged(const QTextCharFormat &format)
{
	fontChanged(format.font());
	colorChanged(format.foreground().color());
}

void RichTextEditWidget::cursorPositionChanged()
{
	alignmentChanged(textEdit->alignment());
}

void RichTextEditWidget::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
	if (const QMimeData *md = QApplication::clipboard()->mimeData())
		actionPaste->setEnabled(md->hasText());
#endif
}

void RichTextEditWidget::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
	QTextCursor cursor = textEdit->textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);
	textEdit->mergeCurrentCharFormat(format);
}

void RichTextEditWidget::fontChanged(const QFont &f)
{
	comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
	actionTextBold->setChecked(f.bold());
	actionTextItalic->setChecked(f.italic());
	actionTextUnderline->setChecked(f.underline());
}

void RichTextEditWidget::colorChanged(const QColor &c)
{
	QPixmap pix(16, 16);
	pix.fill(c);
	actionTextColor->setIcon(pix);
}

void RichTextEditWidget::alignmentChanged(Qt::Alignment a)
{
	if (a & Qt::AlignLeft)
		actionAlignLeft->setChecked(true);
	else if (a & Qt::AlignHCenter)
		actionAlignCenter->setChecked(true);
	else if (a & Qt::AlignRight)
		actionAlignRight->setChecked(true);
	else if (a & Qt::AlignJustify)
		actionAlignJustify->setChecked(true);
}

QToolBar * RichTextEditWidget::addToolBar(QString caption) {
	QVBoxLayout * lay = dynamic_cast<QVBoxLayout *>(layout());
	QToolBar * b = new QToolBar(caption, this);
	lay->insertWidget(lay->count()-1, b);
	return b;
}

void RichTextEditWidget::undo() { textEdit->undo(); }
void RichTextEditWidget::redo() { textEdit->redo(); }
void RichTextEditWidget::cut() { textEdit->cut(); }
void RichTextEditWidget::copy() { textEdit->copy(); }
void RichTextEditWidget::paste() { textEdit->paste(); }


} // namespace QtExt
