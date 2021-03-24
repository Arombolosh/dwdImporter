#include "DWDDelegate.h"

#include <QApplication>
#include <QStyleOptionButton>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>




DWDDelegate::DWDDelegate( QObject *parent )
	: QStyledItemDelegate( parent )
{
}

DWDDelegate::~DWDDelegate()
{
}

void DWDDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{

	QStyleOptionButton opt;// Setting parameters
	Qt::ItemFlags flags = index.flags();
	Qt::CheckState state = (Qt::CheckState)index.data( Qt::CheckStateRole ).toInt();
	bool isSelectable = flags.testFlag(Qt::ItemIsUserCheckable);
	if (!isSelectable)
		return;

	opt.state = QStyle::State_Enabled; // CheckBox enabled
	if ( option.state & QStyle::State_MouseOver )
		opt.state |= QStyle::State_MouseOver; // Mouse over sell
	if ( state == Qt::Checked )
		opt.state |= QStyle::State_On;
	else
		opt.state |= QStyle::State_Off;

	// Check box rect
	opt.rect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &opt, nullptr );
	const int x = option.rect.center().x() - opt.rect.width() / 2;
	const int y = option.rect.center().y() - opt.rect.height() / 2;
	opt.rect.moveTo( x, y );

	// Mandatory: drawing check box
	QApplication::style()->drawControl( QStyle::CE_CheckBox, &opt, painter );
}

//bool DWDDelegate::editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) {

//#if 0
//	switch ( event->type() )
//	{
//	case QEvent::MouseButtonPress:
//		m_lastClickedIndex = index;
//		break;

//	case QEvent::MouseButtonRelease:
//	{
//		if ( index != m_lastClickedIndex )
//			break;
//		QMouseEvent *e = static_cast< QMouseEvent * >( event );
//		if ( e->button() != Qt::LeftButton )
//			break;
//		m_lastClickedIndex = QModelIndex();

//		QStyleOptionButton opt;
//		opt.rect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &opt, NULL );
//		const int x = option.rect.center().x() - opt.rect.width() / 2;
//		const int y = option.rect.center().y() - opt.rect.height() / 2;
//		opt.rect.moveTo( x, y );

//		if ( opt.rect.contains( e->pos() ) )
//		{
//			// TODO: process click on checkbox logic
//			Qt::CheckState state = (Qt::CheckState)index.data( Qt::CheckStateRole ).toInt();
//			switch ( state )
//			{
//			case Qt::Unchecked:
//				state = Qt::PartiallyChecked;
//				break;
//			case Qt::PartiallyChecked:
//				state = Qt::Checked;
//				break;
//			case Qt::Checked:
//				state = Qt::Unchecked;
//				break;
//			}

//			model->setData( index, state, Qt::CheckStateRole );
//		}
//		return true;
//	}

//	default:
//		break;
//	}
//#endif

//	return QAbstractItemDelegate::editorEvent( event, model, option, index );
//}

