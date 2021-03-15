/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_NorthPanelH
#define QtExt_NorthPanelH

#include <QWidget>

#include <QMutex>
#include <QPixmap>


class QHBoxLayout;
class QButtonGroup;
class QLabel;
class QProgressBar;

namespace QtExt {
	class IconButton;
}

namespace QtExt {

	class PushButton;


class NorthPanel : public QWidget {
	Q_OBJECT
public:
	explicit NorthPanel( QWidget * parent = 0 );
	~NorthPanel();

	/*! Adds a view button to the horizontal layout. */
	void addViewButton( PushButton * btn );

	/*! Turns visibility or the progress widget on/off. */
	void setProgressVisible(bool visible);

	/*! Return access to the abort button. */
	QtExt::IconButton * abortButton() const;

	/*! Sets the logo of the user.
		\param userLogo pixmap that keeps the logo. */
	void setLogo( const QPixmap &userLogo  );

signals:

	/*! Emitted when user clicks on the abort button. */
	void abortClicked();



public slots:

	/*! Sets the progress widget caption. */
	void setProgressCaption(const QString & msg);

	/*! Sets the progress bar range and message.
		\param total		The total number of items to process. This value will
							be used as max value of the progress bar with the special
							case of 0, which means the progress bar is to be left untouched.
		\param current		The current item being processed, must not be larger than totalItems.
							Special case: current = -1 means we are done and can hide the
							progress bar again. This also allows "killing" the progress bar
							when an error occurred before current = total.
		\param msg			An optional message string to be displayed.
		*/
	void setProgressInfo(int total, int current, QString msg);



private slots:

	/*! Triggered when user clicks on "Abort task" button. */
	void on_progressAbortButton_clicked();


private:

	/*! Layout for buttons. */
	QHBoxLayout		*m_buttonLayout;

	/*! The widget containing all the progress stuff.
		The progress-related controls are all wrapped in
		the widget to allow for easy hiding/showing and
		better layout control. */
	QWidget			*m_progressWidget;

	/*! Label for the progress widget. */
//	QLabel			*m_progressCaption;

	/*! Label for the progress message. */
//	QLabel			*m_progressMessage;

	/*! The progress bar. */
	QProgressBar	*m_progressBar;

	/*! The abort button for longterm progress things */
	QtExt::IconButton		*m_progressAbortButton;

	/*! logo state guard */
	bool			m_bLogo;

	/*! The label that contains the logo. */
	QLabel			*m_labelLogo;
};


} // namespace QtExt

#endif // QtExt_NorthPanelH
