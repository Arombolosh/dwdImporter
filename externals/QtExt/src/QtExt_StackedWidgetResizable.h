#ifndef QtExt_StackedWidgetResizableH
#define QtExt_StackedWidgetResizableH

#include <QStackedWidget>

namespace QtExt {

/*! \brief Derivation of QStacked widget with changed sizing behaviour.
	Ther base class creates its size from the biggest internal widget. This class uses only the currently visible widget for size.
*/
class StackedWidgetResizable : public QStackedWidget
{
	Q_OBJECT
public:
	/*! Standard constructor. Connects the currentChanged signal with the internal slot.*/
	StackedWidgetResizable(QWidget* parent = nullptr);

	/*! Redefinition of addWidget method in order to change the size policies of the widget to ignored.
		This removes the widget fron sizeHint calculation.
	*/
	void addWidget(QWidget* pWidget);

	/*! Returns sizeHint of current widget.*/
	QSize sizeHint() const override;

	/*! Returns minimumSizeHint of current widget.*/
	QSize minimumSizeHint() const override;

private slots:
	/*! Connected with currentChanged signal in order to set size policy of current widget to preferred.
		It calles adjustSize for recalculation of complete size.
	*/
	void onCurrentChanged(int index);
};

} // end namespace

#endif // QtExt_StackedWidgetResizableH
