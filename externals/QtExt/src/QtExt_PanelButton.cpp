/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#include "QtExt_PanelButton.h"

#include <QDebug>
#include <QApplication>

#include "QtExt_Constants.h"

namespace QtExt {


PanelButton::PanelButton( QWidget * parent, Direction direction ):
	QToolButton( parent )
{

	QIcon iconBtn;

	switch ( direction ) {
		case PanelButton::DD_NORTH :
		{
			setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_top/03_arrows_flat_top_enabled_24x24.png")), QIcon::Normal, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_top/04_arrows_flat_top_hover_24x24.png")), QIcon::Active, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_bottom/05_arrows_flat_bottom_enabled_checked_24x24.png")), QIcon::Normal, QIcon::Off);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_bottom/06_arrows_flat_bottom_hover_checked_24x24.png")), QIcon::Active, QIcon::Off);
			setIcon(iconBtn);
			m_direction = PanelButton::DD_NORTH;
			setMaximumHeight(PANEL_BUTTON_SIZE);
		} break;
		case PanelButton::DD_SOUTH :
			setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_bottom/03_arrows_flat_bottom_enabled_24x24.png")), QIcon::Normal, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_bottom/04_arrows_flat_bottom_hover_24x24.png")), QIcon::Active, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_top/05_arrows_flat_top_enabled_checked_24x24.png")), QIcon::Normal, QIcon::Off);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_top/06_arrows_flat_top_hover_checked_24x24.png")), QIcon::Active, QIcon::Off);
			setIcon(iconBtn);
			m_direction = PanelButton::DD_SOUTH;
			setMaximumHeight(PANEL_BUTTON_SIZE);
			break;
		case PanelButton::DD_EAST :
			setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_right/03_arrows_flat_right_enabled_24x24.png")), QIcon::Normal, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_right/04_arrows_flat_right_hover_24x24.png")), QIcon::Active, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_left/05_arrows_flat_left_enabled_checked_24x24.png")), QIcon::Normal, QIcon::Off);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_left/06_arrows_flat_top_hover_checked_24x24.png")), QIcon::Active, QIcon::Off);
			setIcon(iconBtn);
			m_direction = PanelButton::DD_EAST;
			setMaximumWidth(PANEL_BUTTON_SIZE);
			break;
		case PanelButton::DD_WEST :
			setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_left/03_arrows_flat_left_enabled_24x24.png")), QIcon::Normal, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_left/04_arrows_flat_left_hover_24x24.png")), QIcon::Active, QIcon::On);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_right/05_arrows_flat_right_enabled_checked_24x24.png")), QIcon::Normal, QIcon::Off);
			iconBtn.addPixmap(QPixmap(QString::fromUtf8(":/master/arrow_flat_right/06_arrows_flat_right_hover_checked_24x24.png")), QIcon::Active, QIcon::Off);
			setIcon(iconBtn);
			m_direction = PanelButton::DD_WEST;
			setMaximumWidth(PANEL_BUTTON_SIZE);
			break;

	}

	setCheckable(true);
	setAutoRaise(true);
	retranslateUi();
}


void PanelButton::retranslateUi() {

#if QT_VERSION >= 0x050000
// Qt5 code
	switch(m_direction){
		case PanelButton::DD_WEST:
			setShortcut(QApplication::translate("PanelButton", "Alt+3", 0));
		break;
		case PanelButton::DD_EAST:
			setShortcut(QApplication::translate("PanelButton", "Alt+4", 0));
			break;
		case PanelButton::DD_SOUTH:
			setShortcut(QApplication::translate("PanelButton", "Alt+2", 0));
			break;
		case PanelButton::DD_NORTH:
			setShortcut(QApplication::translate("PanelButton", "Alt+1", 0));
			break;
	}
#else
// Qt4 code
	switch(m_direction){
		case PanelButton::DD_WEST:
			setShortcut(QApplication::translate("PanelButton", "Alt+3", 0, QApplication::UnicodeUTF8));
		break;
		case PanelButton::DD_EAST:
			setShortcut(QApplication::translate("PanelButton", "Alt+4", 0, QApplication::UnicodeUTF8));
			break;
		case PanelButton::DD_SOUTH:
			setShortcut(QApplication::translate("PanelButton", "Alt+2", 0, QApplication::UnicodeUTF8));
			break;
		case PanelButton::DD_NORTH:
			setShortcut(QApplication::translate("PanelButton", "Alt+1", 0, QApplication::UnicodeUTF8));
			break;
	}
#endif


}

} // namespace QtExt
