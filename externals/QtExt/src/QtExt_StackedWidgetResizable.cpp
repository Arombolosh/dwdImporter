#include "QtExt_StackedWidgetResizable.h"

namespace QtExt {

StackedWidgetResizable::StackedWidgetResizable(QWidget* parent) :
	QStackedWidget(parent)
{
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
}

void StackedWidgetResizable::addWidget(QWidget* pWidget) {
	pWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	QStackedWidget::addWidget(pWidget);
}

void StackedWidgetResizable::onCurrentChanged(int index) {
	QWidget* pWidget = widget(index);
	Q_ASSERT(pWidget != nullptr);
	pWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	pWidget->adjustSize();
	adjustSize();
}

QSize StackedWidgetResizable::sizeHint() const {
	return currentWidget()->sizeHint();
}

QSize StackedWidgetResizable::minimumSizeHint() const {
	return currentWidget()->minimumSizeHint();
}

} // end namespace
