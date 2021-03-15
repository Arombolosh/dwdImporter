#ifndef TableTestMainH
#define TableTestMainH

#include <QMainWindow>

class QPrintPreviewWidget;
class QTextDocument;

namespace QtExt {
	class Table;
	class TableWidget;
}

namespace Ui {
	class TableTestMain;
}

class TableTestMain : public QMainWindow {
	Q_OBJECT
public:
	TableTestMain(QWidget *parent = 0);
	~TableTestMain();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::TableTestMain *ui;
	QTextDocument*	m_textDocument;
	QFont			m_normalFont;
	QtExt::Table*	m_table;
	QPrinter*				m_printer;
	QPrintPreviewWidget*	m_previewWidget;
	QtExt::TableWidget*			m_tableWidget;
	QRectF					m_pageRect;
	bool					m_usePreviewWidget;

	void setTable();
	void setTable2();
	void setTable3();

private slots:
	void on_doubleSpinBoxZoom_valueChanged(double );
	void on_pushButtonPrint_clicked();
	void on_doubleSpinBoxSpacing_valueChanged(double );
	void on_buttonBox_rejected();
	void on_buttonBox_accepted();
	void on_actionClose_triggered();
	void paintPreview(QPrinter* printer);
	void update();
	void setTableRect();
};

#endif // TableTestMainH
