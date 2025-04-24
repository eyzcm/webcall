#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QColor>
#include <QStringList>
#include "data\YLData\YL_TreeViewItem_CommonWord.h"
#include "data\YLData\YL_TreeViewItemLeafNodeData_CommonWord.h"

class UI_QuickCommonWord_ListViewItemModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	UI_QuickCommonWord_ListViewItemModel(QStringList headers, QObject *parent = nullptr);
	~UI_QuickCommonWord_ListViewItemModel();

public:
	QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
	YL_TreeViewItem_CommonWord *itemFromIndex(const QModelIndex &index) const;
	YL_TreeViewItem_CommonWord *root();

public:
	void AddQuickCommonWord(MCommonWordData* objData);
	void ClearAll();

private:
	QStringList m_headers;   //表头内容
	YL_TreeViewItem_CommonWord *m_rootItem;    //根节点
};
