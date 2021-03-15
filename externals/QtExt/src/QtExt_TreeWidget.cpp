#include "QtExt_TreeWidget.h"

namespace QtExt {

int TreeWidget::rowHeight(const QModelIndex &index) const {
	return QTreeView::rowHeight(index);
}

int TreeWidget::rowHeight(const QTreeWidgetItem *item) const {
	return rowHeight(indexFromItem(item));
}

} // namespace QtExt
