#ifndef QtExt_MaterialDatabaseSelectionWidgetH
#define QtExt_MaterialDatabaseSelectionWidgetH

#include <QWidget>
#include <QList>
#include <QItemSelection>

#include <memory>

#include "QtExt_MaterialBase.h"

namespace QtExt {

class MaterialTableProxyModel;
class MaterialTableModel;
class MaterialDBItemDelegate;

namespace Ui {
class MaterialDatabaseSelectionWidget;
}

/*! \brief Basic widget for selecting materials from a database.
   It only contains viewing and filtering functions. Adding or removing materials from database has to be done outside.
*/
class MaterialDatabaseSelectionWidget : public QWidget
{
	Q_OBJECT

public:
	/*! Standard constructor.*/
	explicit MaterialDatabaseSelectionWidget(const QSet<MaterialBase::parameter_t>& visibleParams,
											 const QString& orgName, const QString& programName, const QString& widgetName,
											 QWidget *parent = 0);

	/*! Standard destructor only delete ui.*/
	~MaterialDatabaseSelectionWidget() override;

	/*! Fill the material list with pointer to MaterialBase.
		The model takes ownership of this pointer.
		MaterialBase is abstract. Therefore each caller has to create own derivated classes.
	*/
	void setMaterials(const std::vector<MaterialBase*>&	materials);

	/*! Set selection to material with given id.*/
	void selectMaterial(int id);

	/*! Set view to use default color setting (\sa QtExt::Style).*/
	void setDefaultColors();

	/*! Set colors for user defined color setting.*/
	void setUserDefinedColors(const QColor& bright, const QColor& dark);

	/*! Set color for not built-in materials.*/
	void setUserColor(const QColor& col);

	/*! Set use of alternating colors for rows in view.
		Default setting is on.
		Colors can be set by using setUserColors or setDefaultColors.
		\param alternate If true alternating colors are used.
	*/
	void setAlternatingColors(bool alternate);

	/*! Sets a new filter text. */
	void setMaterialNameFilter(const QString & filterText);

	/*! Sets a new filter text. */
	void setProducerFilter(const QString & filterText);

	static QString parameterDescription(MaterialBase::parameter_t type);

public slots:
	/*! remove material with given id from internal list.
		Nothing will happen if id doesn't exist.
		Must be called if this material is also removed from parent database.
	*/
	void removeMaterial(int id);

	/*! Add given material to internal list.
		Id must be unique. In case of already existing id function return false.
		Must be called if this material is also added to parent database.
	*/
	bool addMaterial(MaterialBase*);

	/*! Set given material to position of existing material with same id.
		Return false if id doesn't exist.
		Must be called if this material is also changed in parent database.
	*/
	bool changeMaterial(MaterialBase*);

signals:
	void materialSelected(int id);
	void doubleClickMaterials(int id);

protected:
	/*! Re-implemented to resize columns based on content, which is only done on first view,
		afterwards user can adjust column sizes at will.
	*/
	void  showEvent(QShowEvent *event) override;

private slots:

	/*! Connected to selction signal of proxy model.*/
	void onMaterialSelectionChanged(const QItemSelection &selected, const QItemSelection &);

	/*! Connected to selction signal of proxy model.*/
	void onMaterialDoubleClicked(QModelIndex index);

	/*! Triggered by category combo.*/
	void on_comboBoxCategories_currentIndexChanged(int index);

	/*! Triggered by name filter edit field.*/
	void on_lineEditMaterialNameFilter_textChanged(const QString &arg1);

	/*! Triggered by producer filter edit field.*/
	void on_lineEditProducerFilter_textChanged(const QString &arg1);

	/*! Is triggerd when a column size is changed.*/
	void columnSizeChanged(int logicalIndex, int oldSize, int newSize);

	/*! Is triggerd when the user clicks at a column header.*/
	void columnClicked(int logicalIndex);

private:

	Ui::MaterialDatabaseSelectionWidget *	ui;
	/*! Model for material table view.*/
	MaterialTableModel*						m_model;
	MaterialTableProxyModel*				m_proxyModel;
	MaterialDBItemDelegate *				m_delegate;
	std::map<MaterialBase::parameter_t,int>	m_parameterSizeMap;
	std::vector<MaterialBase::parameter_t>	m_columnToParameter;
	QString									m_orgName;			///< Organization name. Used for handling settings
	QString									m_programName;		///< program name. Used for handling settings
	QString									m_widgetName;		///< program name. Used for handling settings
	int										m_nameColumn;
};


} // namespace QtExt
#endif // QtExt_MaterialDatabaseSelectionWidgetH
