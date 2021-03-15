#ifndef QtExt_TreeWidgetH
#define QtExt_TreeWidgetH

#include <QTreeWidget>

namespace QtExt {

class TreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	explicit TreeWidget(QWidget *parent = nullptr) :
		QTreeWidget(parent)
	{}

	/*! Redefinition as public method.*/
	int rowHeight(const QModelIndex &index) const;

	/*! Try to get row height of a certain item.*/
	int rowHeight(const QTreeWidgetItem *item) const;
};

} // namespace QtExt

#endif // QtExt_TreeWidgetH
