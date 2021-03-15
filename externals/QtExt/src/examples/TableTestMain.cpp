#ifdef _MSC_VER
	#ifndef NDEBUG
		#include <vld.h>
	#endif
#endif

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QDebug>
#include <QPrintPreviewWidget>

#include "TableTestMain.h"
#include "ui_TableTestMain.h"
#include "QtExt_Table.h"
#include "QtExt_TextFrame.h"
#include "QtExt_TableWidget.h"

TableTestMain::TableTestMain(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::TableTestMain),
	m_textDocument(new QTextDocument),
	m_table(0),
	m_printer(0),
	m_previewWidget(0),
	m_usePreviewWidget(false)
{
	ui->setupUi(this);

	// This is the printer used for the preview widget and the printer.

	m_normalFont = QFont("Arial", 8);
	m_normalFont.setKerning(false);
	m_textDocument->setDefaultFont(m_normalFont);
	m_table = new QtExt::Table(m_textDocument, QSize(100, -1), this);

	ui->verticalLayout->removeWidget(ui->widgetTable);
	delete ui->widgetTable;
	if( m_usePreviewWidget) {
		m_printer = new QPrinter(QPrinter::HighResolution);
		// set default margins
		m_printer->setPageMargins(20, 20, 20, 20, QPrinter::Millimeter);
		m_pageRect = m_printer->pageRect();
		// create print preview widget
		m_previewWidget = new QPrintPreviewWidget(m_printer, this);
		// replace dummy widget with PrintPreviewWidget.
		ui->verticalLayout->insertWidget(0, m_previewWidget);
		ui->doubleSpinBoxSpacing->setValue(m_table->spacing());
		ui->doubleSpinBoxZoom->setValue(m_previewWidget->zoomFactor() * 100);
		connect(m_previewWidget, SIGNAL(paintRequested(QPrinter*)), this, SLOT(paintPreview(QPrinter*)));
	//	connect(m_table, SIGNAL(changed()), this, SLOT(update()));
		connect(m_previewWidget, SIGNAL(previewChanged()), this, SLOT(setTableRect()));
	} else {
		m_pageRect = QRect(0,0,700, 295);
		setTable3();
		m_tableWidget = new QtExt::TableWidget(m_table, this);
		m_tableWidget->setGeometry(m_pageRect.toRect());
		// replace dummy widget with PrintPreviewWidget.
		ui->verticalLayout->insertWidget(0, m_tableWidget);
		ui->doubleSpinBoxZoom->setDisabled(true);
	}


//	m_table = ui->widgetTable->table();
//	setTableRect();
//	QRectF rect = m_table->tableRect(m_printer, -1);
}

TableTestMain::~TableTestMain()
{
	delete m_printer;
	delete m_textDocument;
	delete ui;
}

void TableTestMain::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void TableTestMain::on_actionClose_triggered()
{
	close();
}

void TableTestMain::on_buttonBox_accepted()
{
	close();
}

void TableTestMain::on_buttonBox_rejected()
{
	close();
}

void TableTestMain::on_doubleSpinBoxSpacing_valueChanged(double val)
{
	// Spacing anpassen
	m_table->setSpacing(val);
}

void TableTestMain::setTable2() {
	m_table->setColumnsRows(5,4);

	m_table->setDefaultFont(m_normalFont);
	m_table->setOuterFrameWidth(3);
	m_table->mergeCells(0,0,5,1);
	m_table->mergeCells(0,1,2,1);
	m_table->mergeCells(3,1,2,1);

	m_table->setCellText(0,0,"<h2>Winterklima</h2>");
	m_table->cell(0,0).setAlignment(Qt::AlignCenter);
	m_table->setCellText(0,1,"<h3>Klima auf der Warmseite</h3>");
	m_table->cell(0,1).setAlignment(Qt::AlignCenter);
	m_table->setCellText(3,1,"<h3>Klima auf der Kaltseite</h3>");
	m_table->cell(3,1).setAlignment(Qt::AlignCenter);
	m_table->setCellText(0,2,"Temperatur");
	m_table->cell(0,2).setAlignment(Qt::AlignVCenter);
	m_table->setCellText(0,3,"Relative Luftfeuchte");
	m_table->cell(0,3).setAlignment(Qt::AlignVCenter);
	m_table->setCellText(3,2,"Temperatur");
	m_table->cell(3,2).setAlignment(Qt::AlignVCenter);
	m_table->setCellText(3,3,"Relative Luftfeuchte");
	m_table->cell(3,3).setAlignment(Qt::AlignVCenter);

	m_table->setCellText(1,2,"20.0 °C");
	m_table->cell(1,2).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(1,3,"50.0 %");
	m_table->cell(1,3).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(4,2,"-10 °C");
	m_table->cell(4,2).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(4,3,"80.0 %");
	m_table->cell(4,3).setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	m_table->cell(0,1).setBorderWidth(QtExt::TableCell::TopBorder, 2);
	m_table->cell(1,1).setBorderWidth(QtExt::TableCell::TopBorder, 2);
	m_table->cell(1,1).setBorderWidth(QtExt::TableCell::RightBorder, 2);
	m_table->cell(1,2).setBorderWidth(QtExt::TableCell::RightBorder, 2);
	m_table->cell(1,3).setBorderWidth(QtExt::TableCell::RightBorder, 2);

	m_table->cell(3,1).setBorderWidth(QtExt::TableCell::TopBorder, 2);
	m_table->cell(4,1).setBorderWidth(QtExt::TableCell::TopBorder, 2);
	m_table->cell(3,1).setBorderWidth(QtExt::TableCell::LeftBorder, 2);
	m_table->cell(3,2).setBorderWidth(QtExt::TableCell::LeftBorder, 2);
	m_table->cell(3,3).setBorderWidth(QtExt::TableCell::LeftBorder, 2);

	m_table->cell(2,0).setBorderWidth(QtExt::TableCell::BottomBorder, 0);
	m_table->cell(2,1).setBorderWidth(QtExt::TableCell::TopBorder, 0);
	m_table->cell(2,1).setBorderWidth(QtExt::TableCell::BottomBorder, 0);
	m_table->cell(2,2).setBorderWidth(QtExt::TableCell::TopBorder, 0);
	m_table->cell(2,2).setBorderWidth(QtExt::TableCell::BottomBorder, 0);
	m_table->cell(2,3).setBorderWidth(QtExt::TableCell::TopBorder, 0);

}

void TableTestMain::setTable3() {
	m_table->setColumnsRows(4, 3);
	m_table->setDefaultFont(m_normalFont);
	m_table->setOuterFrameWidth(1.5);
	m_table->setInnerFrameWidth(0.7);
	m_table->setMargin(0);

	m_table->setColumnSizeFormat(0, QtExt::CellSizeFormater::Fixed, 20);
	m_table->setColumnSizeFormat(1, QtExt::CellSizeFormater::Fixed, 200);
	m_table->setColumnSizeFormat(2, QtExt::CellSizeFormater::Fixed, 150);
	m_table->setCellText(1,0,QString("<h3>%1</h3>").arg(tr("Material wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")));
	m_table->setCellText(2,0,"<h3>d [mm]</h3>");

	m_table->setCellText(0,1,QString("%L1").arg(1));
	m_table->setCellText(1,1,"Anbindung Fachwerkwärmedämmlehm mit einem richtig langen Namen um zu testen wie der Zeilenumbruch funktioniert");
	m_table->cell(1,1).setAlignment(Qt::AlignVCenter);
	m_table->setCellText(2,1,QString("%L1").arg(100.0, 0, 'f', 1));
	m_table->cell(2,1).setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	m_table->setCellText(0,2,QString("%L1").arg(2));
	m_table->setCellText(1,2,"Testmaterial");
	m_table->cell(1,2).setAlignment(Qt::AlignVCenter);
	m_table->setCellText(2,2,QString("%L1").arg(100.0, 0, 'f', 1));
	m_table->cell(2,2).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
}

void TableTestMain::setTable() {
	int rows = 3;
	m_table->setColumnsRows(8, rows);
	m_table->setDefaultFont(m_normalFont);
	m_table->setOuterFrameWidth(1.5);
	m_table->setInnerFrameWidth(0.7);
	m_table->setMargin(0);

	qreal width = m_pageRect.width();
	QtExt::TextFrame tempFrame(0);
	tempFrame.setText(QString("%1").arg(rows-1));

	QPaintDevice* paintDevice(m_printer);
	if( !m_usePreviewWidget)
		paintDevice = m_previewWidget;

	qreal twidth = tempFrame.frameRect(paintDevice, width).width();
	m_table->setColumnSizeFormat(0, QtExt::CellSizeFormater::Fixed, twidth + 10);
	m_table->setCellText(1,0,QString("<h3>%1</h3>").arg(tr("Material")));
	m_table->setCellText(2,0,"<h3>d [mm]</h3>");
	m_table->setColumnSizeFormat(2, QtExt::CellSizeFormater::AutoMinimum);
	m_table->setCellText(3,0,QString("<h3>%1 [W/mK]</h3>").arg(QString::fromWCharArray(L"\u03BB")));
	m_table->setColumnSizeFormat(3, QtExt::CellSizeFormater::AutoMinimum);
	m_table->setCellText(4,0,QString("<h3>%1 [---]</h3>").arg(QString::fromWCharArray(L"\u03BC")));
	m_table->setColumnSizeFormat(4, QtExt::CellSizeFormater::AutoMinimum);
	m_table->setCellText(5,0,QString("<h3>w<sub>80</sub> [m<sup>3</sup>/m<sup>3</sup>]</h3>"));
	m_table->setColumnSizeFormat(5, QtExt::CellSizeFormater::AutoMinimum);
	m_table->setCellText(6,0,QString("<h3>w<sub>sat</sub> [m<sup>3</sup>/m<sup>3</sup>]</h3>"));
	m_table->setColumnSizeFormat(6, QtExt::CellSizeFormater::AutoMinimum);
	QString AwUnit = trUtf8("kg/m<sup>2</sup>h<sup>%1</sup>").arg(QString::fromWCharArray(L"\u00BD"));
	m_table->setCellText(7,0,QString("<h3>A<sub>w</sub> [%1]</h3>").arg(AwUnit));

	int i = 1;
	m_table->setCellText(0,i,QString("%L1").arg(i));
	m_table->cell(0,i).setAlignment(Qt::AlignCenter);
	m_table->cell(0,i).setBackgroundColor(Qt::blue);
	m_table->setCellText(1,i,"Anbindung Fachwerkwärmedämmlehm mit einem richtig langen Namen um zu testen wie der Zeilenumbruch funktioniert");
	m_table->cell(1,i).setAlignment(Qt::AlignVCenter);
	m_table->setCellText(2,i,QString("%L1").arg(100.0, 0, 'f', 1));
	m_table->cell(2,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(3,i,QString("%L1").arg(0.19, 0,'f', 4));
	m_table->cell(3,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(4,i,QString("%L1").arg(15.0, 0,'f', 1));
	m_table->cell(4,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(5,i,QString("%L1").arg(0.017, 0,'f', 4));
	m_table->cell(5,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(6,i,QString("%L1").arg(0.554, 0,'f', 4));
	m_table->cell(6,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(7,i,QString("%L1").arg(0.9, 0,'f', 4));
	m_table->cell(7,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	i = 2;
	m_table->setCellText(0,i,QString("%L1").arg(i));
	m_table->cell(0,i).setAlignment(Qt::AlignCenter);
	m_table->cell(0,i).setBackgroundColor(Qt::blue);
	m_table->setCellText(1,i,"Altbauklinker");
	m_table->cell(1,i).setAlignment(Qt::AlignVCenter);
	m_table->setCellText(2,i,QString("%L1").arg(100.0, 0, 'f', 1));
	m_table->cell(2,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(3,i,QString("%L1").arg(0.995, 0,'f', 4));
	m_table->cell(3,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(4,i,QString("%L1").arg(45.0, 0,'f', 1));
	m_table->cell(4,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(5,i,QString("%L1").arg(0.004, 0,'f', 4));
	m_table->cell(5,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(6,i,QString("%L1").arg(0.241, 0,'f', 4));
	m_table->cell(6,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	m_table->setCellText(7,i,QString("%L1").arg(1.08, 0,'f', 4));
	m_table->cell(7,i).setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	QRectF rect = m_table->tableRect(paintDevice, width);
}

void TableTestMain::on_pushButtonPrint_clicked()
{
	if( m_usePreviewWidget) {
//	for( int i=0; i<40; ++i) {
//		m_table->clear();
		setTable();
		update();
//	}
//	close();
	// print
//	m_previewWidget->print();
	}
}

void TableTestMain::paintPreview(QPrinter* printer) {
	QPainter prpainter;
	prpainter.begin(printer);
	m_table->drawTable(&prpainter, QPointF(0, 0));
	prpainter.end();
}


void TableTestMain::on_doubleSpinBoxZoom_valueChanged(double val) {
	if(m_previewWidget )
		m_previewWidget->setZoomFactor(val / 100);
}

void TableTestMain::update()  {
	if( m_usePreviewWidget)
		m_previewWidget->updatePreview();
	else
		m_tableWidget->repaintTable();
}

void TableTestMain::setTableRect() {
	if(m_pageRect != m_printer->pageRect(QPrinter::DevicePixel)) {
		m_pageRect = m_printer->pageRect(QPrinter::DevicePixel);
		m_pageRect.setWidth(m_pageRect.width() - 20);
		m_table->setTableSize(m_pageRect.size().toSize());
	}
}

