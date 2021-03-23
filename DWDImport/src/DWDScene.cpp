#include "DWDScene.h"

DWDScene::DWDScene(QObject *parent)
	: QAbstractItemModel(parent)
{
}

QVariant DWDScene::headerData(int section, Qt::Orientation orientation, int role) const
{
	// FIXME: Implement me!
}

QModelIndex DWDScene::index(int row, int column, const QModelIndex &parent) const
{
	// FIXME: Implement me!
}

QModelIndex DWDScene::parent(const QModelIndex &index) const
{
	// FIXME: Implement me!
}

int DWDScene::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 0;

	// FIXME: Implement me!
}

int DWDScene::columnCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 0;

	// FIXME: Implement me!
}

QVariant DWDScene::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	// FIXME: Implement me!
	return QVariant();
}
