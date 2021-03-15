/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_RichTextEditWidgetH
#define QtExt_RichTextEditWidgetH

#include <QWidget>
#include <QMap>
#include <QPointer>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFontComboBox;
class QTextCharFormat;
class QToolBar;
QT_END_NAMESPACE

namespace QtExt {


class SignalingTextEdit : public QTextEdit {
	Q_OBJECT
public:
	SignalingTextEdit(QWidget *parent) : QTextEdit(parent) {}

protected:
	/*! Emits the editingFinished() signal. */
	virtual void focusOutEvent(QFocusEvent * /*event*/) {
		emit editingFinished();
	}

signals:
	void editingFinished();
};

/*! A truncated version of the rich text edit example from Qt.
	Basically a widget with toolbars that allows rich text editing.
*/
class RichTextEditWidget : public QWidget {
	Q_OBJECT

public:
	RichTextEditWidget(QWidget *parent = nullptr);

	SignalingTextEdit *textEdit;

	/*! Returns the current HTML text in the widget without the DOCTYPE stuff in front of <html>. */
	QString htmlText() const;

signals:
	/*! Emitted, when user finishes editing (widget looses focus). */
	void editingFinished();

//protected:
//	/*! Emits the editingFinished() signal. */
//	virtual void focusOutEvent(QFocusEvent *event);
private slots:
	void textBold();
	void textUnderline();
	void textItalic();
	void textFamily(const QString &f);
	void textSize(const QString &p);
	void textStyle(int styleIndex);
	void textColor();
	void textAlign(QAction *a);

	void currentCharFormatChanged(const QTextCharFormat &format);
	void cursorPositionChanged();
	void clipboardDataChanged();

	void undo();
	void redo();
	void cut();
	void copy();
	void paste();

private:
	void setupEditActions();
	void setupTextActions();

	QToolBar * addToolBar(QString caption);

	void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
	void fontChanged(const QFont &f);
	void colorChanged(const QColor &c);
	void alignmentChanged(Qt::Alignment a);

	QAction *actionTextBold;
	QAction *actionTextUnderline;
	QAction *actionTextItalic;
	QAction *actionTextColor;
	QAction *actionAlignLeft;
	QAction *actionAlignCenter;
	QAction *actionAlignRight;
	QAction *actionAlignJustify;
	QAction *actionUndo;
	QAction *actionRedo;
#ifndef QT_NO_CLIPBOARD
	QAction *actionCut;
	QAction *actionCopy;
	QAction *actionPaste;
#endif

	QComboBox *comboListFormat;
	QFontComboBox *comboFont;
	QComboBox *comboSize;

	QToolBar *tb;
};

} // namespace QtExt

#endif // QtExt_RichTextEditWidgetH
