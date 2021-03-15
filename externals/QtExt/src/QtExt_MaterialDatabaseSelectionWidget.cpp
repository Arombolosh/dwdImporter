#include "QtExt_MaterialDatabaseSelectionWidget.h"
#include "ui_QtExt_MaterialDatabaseSelectionWidget.h"

#include <QScrollBar>
#include <QSettings>

#include "QtExt_MaterialTableModel.h"
#include "QtExt_MaterialTableProxyModel.h"
#include "QtExt_MaterialCategory.h"
#include "QtExt_MaterialBase.h"
#include "QtExt_MaterialDBItemDelegate.h"
#include "QtExt_Style.h"

namespace QtExt {

QString MaterialDatabaseSelectionWidget::parameterDescription(MaterialBase::parameter_t type) {
	switch (type) {
		case MaterialBase::MC_ID				: return "MaterialID";
		case MaterialBase::MC_PARAMETRIZATION	: return "MaterialParaView";
		case MaterialBase::MC_NAME				: return "MaterialName";
		case MaterialBase::MC_CATEGORY			: return "MaterialCategory";
		case MaterialBase::MC_PRODUCTID			: return "MaterialProductID";
		case MaterialBase::MC_PRODUCER			: return "MaterialProducer";
		case MaterialBase::MC_SOURCE			: return "MaterialSource";
		case MaterialBase::MC_RHO				: return "MaterialRho";
		case MaterialBase::MC_CET				: return "MaterialCet";
		case MaterialBase::MC_MEW				: return "MaterialMew";
		case MaterialBase::MC_LAMBDA			: return "MaterialLambda";
		case MaterialBase::MC_W80				: return "MaterialW80";
		case MaterialBase::MC_WSAT				: return "MaterialWSat";
		case MaterialBase::MC_AW				: return "MaterialAw";
		case MaterialBase::MC_SD				: return "MaterialSd";
		case MaterialBase::MC_KAir				: return "MaterialKair";
		default : return QString();
	}
}

MaterialDatabaseSelectionWidget::MaterialDatabaseSelectionWidget(const QSet<MaterialBase::parameter_t>& visibleParams,
																 const QString& orgName, const QString& programName, const QString& widgetName,
																 QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MaterialDatabaseSelectionWidget),
	m_model(new MaterialTableModel(visibleParams, this)),
	m_proxyModel(new MaterialTableProxyModel(this)),
	m_delegate(new MaterialDBItemDelegate(this)),
	m_columnToParameter((size_t)m_model->columnCount(QModelIndex())),
	m_orgName(orgName),
	m_programName(programName),
	m_widgetName(widgetName),
	m_nameColumn(-1)
{
	ui->setupUi(this);

	m_parameterSizeMap[MaterialBase::MC_ID] = 50;
	m_parameterSizeMap[MaterialBase::MC_PARAMETRIZATION] = MaterialTableModel::ParametrizationColWidth;
	m_parameterSizeMap[MaterialBase::MC_NAME] = 230;
	m_parameterSizeMap[MaterialBase::MC_CATEGORY] = 130;
	m_parameterSizeMap[MaterialBase::MC_PRODUCTID] = 200;
	m_parameterSizeMap[MaterialBase::MC_PRODUCER] = 200;
	m_parameterSizeMap[MaterialBase::MC_RHO] = 30;
	m_parameterSizeMap[MaterialBase::MC_CET] = 30;
	m_parameterSizeMap[MaterialBase::MC_MEW] = 30;
	m_parameterSizeMap[MaterialBase::MC_LAMBDA] = 30;
	m_parameterSizeMap[MaterialBase::MC_W80] = 30;
	m_parameterSizeMap[MaterialBase::MC_WSAT] = 30;
	m_parameterSizeMap[MaterialBase::MC_AW] = 30;
	m_parameterSizeMap[MaterialBase::MC_SD] = 30;
	m_parameterSizeMap[MaterialBase::MC_KAir] = 30;

	QSettings settings(m_orgName, m_programName);
	settings.beginGroup(m_widgetName);
	for(auto&& paramSize : m_parameterSizeMap) {
		paramSize.second = settings.value(parameterDescription(paramSize.first) + "Column", paramSize.second).toInt();
	}
	settings.endGroup();

	ui->tableView->verticalHeader()->setVisible(false);
	ui->tableView->horizontalHeader()->setVisible(true);
	ui->tableView->horizontalHeader()->setStretchLastSection(false);
	QFont f = qApp->font();
	QFontMetrics fm(f);
	int pixels = fm.lineSpacing();
	int additionalGap = pixels*0.2;
#ifdef Q_OS_LINUX
	if (additionalGap < 3)
		additionalGap = 3;
#elif defined(Q_OS_MAC)
	if (additionalGap < 2)
		additionalGap = 2;
#else
	additionalGap = pixels*0.4;
	if (additionalGap < 4)
		additionalGap = 4;
#endif
	pixels += additionalGap;
	ui->tableView->verticalHeader()->setDefaultSectionSize(pixels);

	ui->tableView->setItemDelegate(m_delegate);

	m_proxyModel->setSourceModel(m_model);
	ui->tableView->setModel(m_proxyModel);
	ui->tableView->setSortingEnabled(true);
	ui->tableView->sortByColumn(1, Qt::AscendingOrder);

	// set horizontal header
	QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
	horizontalHeader->setHighlightSections(false);
	horizontalHeader->setMinimumSectionSize(30);
	// set section size according column selection
	{
		int i=0;
		for(int p=0; p<MaterialBase::NUM_MC; ++p) {
			MaterialBase::parameter_t para = static_cast<MaterialBase::parameter_t>(p);
			if(visibleParams.contains(para)) {
				m_columnToParameter[(size_t)i] = para;
				int size = m_parameterSizeMap[para];
				ui->tableView->setColumnWidth(i, size);
				if(para == MaterialBase::MC_NAME)
					m_nameColumn = i;
				++i;
			}
		}
		m_model->setDefaultRowHeight(ui->tableView->verticalHeader()->defaultSectionSize());
	}
	ui->comboBoxCategories->blockSignals(true);
	ui->comboBoxCategories->addItem(tr("All"));
	for (int i=0; i<MaterialCategory::NUM_CATEGORIES; ++i) {
		ui->comboBoxCategories->addItem(MaterialCategory::name(static_cast<MaterialCategory::category_t>(i)));
	}
	ui->comboBoxCategories->blockSignals(false);

	if(!visibleParams.contains(MaterialBase::MC_PRODUCER)) {
		ui->lineEditProducerFilter->setVisible(false);
		ui->labelProducerFilterCaption->setVisible(false);
	}

	// reacts on selection change of the view and transfor index to QModelIndex
	connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			this,							 SLOT(onMaterialSelectionChanged(QItemSelection,QItemSelection)));

	connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)),
			this,  SLOT(onMaterialDoubleClicked(QModelIndex)));
}


MaterialDatabaseSelectionWidget::~MaterialDatabaseSelectionWidget() {

	// store current view index and column widths
	QSettings settings(m_orgName, m_programName);
	settings.beginGroup(m_widgetName);
	for(size_t i = 0; i<m_columnToParameter.size(); ++i) {
		settings.setValue(parameterDescription(m_columnToParameter[i]) + "Column", ui->tableView->columnWidth((int)i));
	}
	delete ui;
	settings.endGroup();
}


void  MaterialDatabaseSelectionWidget::showEvent(QShowEvent *) {
	int totalColumnWidth = 0;
	for(int i=0; i<m_model->columnCount(); ++i) {
		totalColumnWidth += ui->tableView->columnWidth(i);
	}
	int innerWidth = ui->tableView->viewport()->size().width() - ui->tableView->verticalScrollBar()->sizeHint().width();
	int nameColWidth = innerWidth - totalColumnWidth;
	if(m_nameColumn > -1 && nameColWidth > 0)
		ui->tableView->setColumnWidth(m_nameColumn, nameColWidth);
}


void MaterialDatabaseSelectionWidget::setMaterials(const std::vector<MaterialBase*>&	materials) {
	m_model->setMaterials(materials);
}

void MaterialDatabaseSelectionWidget::onMaterialSelectionChanged(const QItemSelection &selected, const QItemSelection &) {
	ui->textEditAdditional->clear();
	if( selected.indexes().isEmpty()) {
		emit materialSelected(-1);
		ui->labelSelectedMaterial->setText(tr("Nothing"));
		return;
	}

	QModelIndex index = selected.indexes().front();
	// directly tell the proxymodel to change the selection
	QModelIndex sindex = m_proxyModel->mapToSource(index);
	MaterialBase* mat = static_cast<MaterialBase*>(sindex.internalPointer());

	if(mat != nullptr) {
		QStringList notesList = mat->notes().split('\n');
		for (QString str : notesList) {
			ui->textEditAdditional->append(str);
		}

		ui->textEditAdditional->verticalScrollBar()->triggerAction(QScrollBar::SliderToMinimum);
		ui->textEditAdditional->moveCursor(QTextCursor::Start);
		emit materialSelected(mat->materialId());

		ui->labelSelectedMaterial->setText(QString("%1 - %2").arg(mat->materialId()).arg(mat->name()));
	}
	else {
		emit materialSelected(-1);
		ui->labelSelectedMaterial->setText(tr("Nothing"));
	}
}


void MaterialDatabaseSelectionWidget::onMaterialDoubleClicked(QModelIndex index) {
	QModelIndex sindex = m_proxyModel->mapToSource(index);
	MaterialBase* mat = static_cast<MaterialBase*>(sindex.internalPointer());
	if(mat != nullptr)
		emit doubleClickMaterials(mat->materialId());
	else
		emit doubleClickMaterials(-1);
}


void MaterialDatabaseSelectionWidget::on_comboBoxCategories_currentIndexChanged(int index) {
	m_proxyModel->setCategoryFilter(index-1);
	QModelIndexList selIndexes = ui->tableView->selectionModel()->selectedRows();
	if (!selIndexes.isEmpty()) {
		QModelIndex proxyIdx = m_proxyModel->mapFromSource( selIndexes.first());
		if (!proxyIdx.isValid())
			ui->textEditAdditional->clear();
	}
	else {
		ui->textEditAdditional->clear();
	}
}

void MaterialDatabaseSelectionWidget::on_lineEditMaterialNameFilter_textChanged(const QString &arg1) {
	m_proxyModel->setNameFilter(arg1);
}

void QtExt::MaterialDatabaseSelectionWidget::on_lineEditProducerFilter_textChanged(const QString &arg1) {
	m_proxyModel->setProducerFilter(arg1);
}

void MaterialDatabaseSelectionWidget::columnSizeChanged(int, int, int) {
//	QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
//	horizontalHeader->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void MaterialDatabaseSelectionWidget::columnClicked(int index) {
//	ui->tableView->resizeColumnToContents(index);
}

void MaterialDatabaseSelectionWidget::selectMaterial(int id) {
	int row = m_proxyModel->getRow(id);
	if( row == -1)
		row = 0;
	ui->tableView->selectRow(row);
	ui->tableView->scrollTo( ui->tableView->model()->index(row, 0));
}

void MaterialDatabaseSelectionWidget::setDefaultColors() {
	m_delegate->setDefaultColors();
}

void MaterialDatabaseSelectionWidget::setUserDefinedColors(const QColor& bright, const QColor& dark) {
	m_delegate->setUserDefinedColors(bright, dark);
}

void MaterialDatabaseSelectionWidget::setUserColor(const QColor& col) {
	m_delegate->setUserColor(col);
}

void MaterialDatabaseSelectionWidget::setAlternatingColors(bool alternate) {
	m_delegate->setAlternatingColors(alternate);
}

void MaterialDatabaseSelectionWidget::setMaterialNameFilter(const QString & filterText) {
	ui->lineEditMaterialNameFilter->setText(filterText);
}

void MaterialDatabaseSelectionWidget::setProducerFilter(const QString & filterText) {
	ui->lineEditProducerFilter->setText(filterText);
}

void MaterialDatabaseSelectionWidget::removeMaterial(int id) {
	m_model->removeMaterial(id);
}

bool MaterialDatabaseSelectionWidget::addMaterial(MaterialBase* material) {
	return m_model->addMaterial(material);
}

bool MaterialDatabaseSelectionWidget::changeMaterial(MaterialBase* material) {
	return m_model->changeMaterial(material);
}

} // namespace QtExt
