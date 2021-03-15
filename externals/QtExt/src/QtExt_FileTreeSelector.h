#ifndef QtExt_FileTreeSelectorH
#define QtExt_FileTreeSelectorH

#include <QWidget>
#include <QTreeWidget>
#include <QDir>


namespace QtExt {

/*! \brief Class contains filename, path and splitted path.
*/
struct FileWrapper {
	/*! Standard constructor.*/
	FileWrapper() :
		m_temporary(false)
	{}
	/*! Filename without path and extension.*/
	QString		m_name;
	/*! Filename without path.*/
	QString		m_filename;
	/*! List of category strings which will be created from directory structure.*/
	QStringList	m_categories;
	/*! Complete description of file.*/
	QFileInfo	m_file;
	/*! If true its a temporary file.*/
	bool m_temporary;
};

/*! \brief Tree view for selecting files based on a given root path.
	A filter for specific filenames and/or file extensions can be set.
*/
class FileTreeSelector : public QWidget
{
	Q_OBJECT
public:

	enum Option {
		ShowExtensions,			///< Show filename with extension.
		CanSelectDirectories,	///< Selection of directories is allowed
		MultiSelection			///< Selection of multiple files allowed
	};

	/*! Standard constructor.*/
	explicit FileTreeSelector(QWidget *parent = nullptr);

	/*! Return the currently selected files.*/
	QStringList selectedFiles() const;

	/*! Set root path as start for file search.*/
	void setRootPath(const QString& path, bool forceUpdate = true);

	/*! Set extensions as filter.
		\param filterExtensions List of extension (with or without point).
		\param forceUpdate If true treeview will be updated after set of new filters.
	*/
	void setExtensionsFilter(const QStringList& filterExtensions, bool forceUpdate = true);

	/*! Set filenames as filter.
		\param filterFiles List of file names.
		\param forceUpdate If true treeview will be updated after set of new filters.
	*/
	void setFileFilter(const QStringList& filterFiles, bool forceUpdate = true);

	/*! Set the option \a opt to the value \a isSet.*/
	void setOption(Option opt, bool isSet);

	/*! Clear all current selections.*/
	void clearSelections();

	/*! Add a file to the list of files in order to add these temperary to the list.*/
	void addFileToAdd(const QString& filename, const QStringList& categories);

	/*! Clear list of temporary files.*/
	void clearFilesToAdd();

	/*! Return the current file list. update must be called in order to create a valid list.*/
	const QList<FileWrapper> files() const;

signals:
	void selectionChanged(QStringList files);

public slots:

	/*! Triggerd from tree vie if selection is changed.
		Check if selection is valid and emit signal climateSelected with corresponding climatWrapper object.*/
	void onSelectionChanged();

	/*! Updates the tree view content.*/
	void update();

private:

	/*! Split the directories into a category list.*/
	bool categoriesFromDir(const QDir& filePath, QStringList& res);

	/*! Helper function for finding tree items.*/
	QTreeWidgetItem* findChildByText(QTreeWidgetItem* parent, const QString& text) const;

	QTreeWidget*		m_treeWidget;
	QString				m_rootPath;
	QStringList			m_filter;
	QStringList			m_fileFilter;
	QStringList			m_pathList;
	QList<FileWrapper>	m_files;
	QList<FileWrapper>	m_filesToAdd;
	QStringList			m_selectedFiles;
	QSet<Option>		m_options;
};

} // namespace QtExt

#endif // QtExt_FileTreeSelectorH
