#include "UI_QuickCommonWord_ListViewItemModel.h"
#include "utils/utilshelper.h"
#include <QToolTip>

UI_QuickCommonWord_ListViewItemModel::UI_QuickCommonWord_ListViewItemModel(QStringList headers, QObject *parent)
	: QAbstractItemModel(parent)
{
	m_headers = headers;
	m_rootItem = new YL_TreeViewItem_CommonWord;
}

UI_QuickCommonWord_ListViewItemModel::~UI_QuickCommonWord_ListViewItemModel()
{
	if (m_rootItem != nullptr)
	{
		delete m_rootItem;
		m_rootItem = nullptr;
	}
}

YL_TreeViewItem_CommonWord *UI_QuickCommonWord_ListViewItemModel::itemFromIndex(const QModelIndex &index) const
{
	if (index.isValid())
	{
		YL_TreeViewItem_CommonWord *item = static_cast<YL_TreeViewItem_CommonWord*>(index.internalPointer());
		return item;
	}
	return nullptr;
}

YL_TreeViewItem_CommonWord *UI_QuickCommonWord_ListViewItemModel::root()
{
	return m_rootItem;
}

QVariant UI_QuickCommonWord_ListViewItemModel::headerData(int section, Qt::Orientation orientation,int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if(role == Qt::DisplayRole)
		{
			return m_headers.at(section);
		}
	}
	return QVariant();
}

QVariant UI_QuickCommonWord_ListViewItemModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid())
	{
		YL_TreeViewItem_CommonWord *item = static_cast<YL_TreeViewItem_CommonWord*>(index.internalPointer());
		if (role == Qt::DisplayRole)
		{
			return QVariant();
		}
		else if (role == Qt::UserRole)
		{
			if (item)
			{
				return item->UserData();
			}		
		}
		else if (role == Qt::ToolTipRole)
		{
			if (item)
			{
				QString strData = item->Data();
				if (strData.length()>20)
				{
					QToolTip::showText(QCursor::pos(), strData.left(20).append("..."));
				}
				else
				{
					QToolTip::showText(QCursor::pos(), strData);
				}
			}			
		}
		else if (role == Qt::BackgroundColorRole)
		{
			QColor color;
			if (item->NodeState() == Enum_YL_NodeState_Normal)
			{
				color = QColor("#ffffff");
			}
			else if (item->NodeState() == Enum_YL_NodeState_Hover)
			{
				color = QColor("#eef1f5");
			}
			else if (item->NodeState() == Enum_YL_NodeState_Select)
			{
				color = QColor("#dde0e7");
			}

			return color;
		}
	}
	return QVariant();
}

Qt::ItemFlags UI_QuickCommonWord_ListViewItemModel::flags(const QModelIndex &index) const
{
	if (index.isValid())
	{
		QAbstractItemModel::flags(index);
	}
	return 0;
}

QModelIndex UI_QuickCommonWord_ListViewItemModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	YL_TreeViewItem_CommonWord *parentItem;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<YL_TreeViewItem_CommonWord*>(parent.internalPointer());

	YL_TreeViewItem_CommonWord *childItem = parentItem->Child(row);
	if (childItem)
	{		
		return createIndex(row, column, childItem);
	}
	else
		return QModelIndex();
}

QModelIndex UI_QuickCommonWord_ListViewItemModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

int UI_QuickCommonWord_ListViewItemModel::rowCount(const QModelIndex &parent) const
{
	YL_TreeViewItem_CommonWord *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<YL_TreeViewItem_CommonWord*>(parent.internalPointer());

	return parentItem->ChildCount();

}

int UI_QuickCommonWord_ListViewItemModel::columnCount(const QModelIndex &parent) const
{
	return m_headers.size();
}

void UI_QuickCommonWord_ListViewItemModel::ClearAll()
{
	Q_EMIT layoutAboutToBeChanged();
	m_rootItem->RemoveChilds();
	Q_EMIT layoutChanged();
}

void UI_QuickCommonWord_ListViewItemModel::AddQuickCommonWord(MCommonWordData* objData)
{
	if (objData && m_rootItem)
	{
		YL_TreeViewItemLeafNodeData_CommonWord* itemData = new YL_TreeViewItemLeafNodeData_CommonWord();
		itemData->InitData(*objData, Enum_YL_NodeType_LeafNodePersonal);

		YL_TreeViewItem_CommonWord* itemObj = new YL_TreeViewItem_CommonWord(m_rootItem);
		itemObj->SetLevel(1);
		itemObj->SetLeaf(true);
		itemObj->SetID(itemData->Id());
		itemObj->SetNodeType(Enum_YL_NodeType_LeafNodePersonal);
		itemObj->SetPtr(itemData);

		Q_EMIT layoutAboutToBeChanged();
		m_rootItem->AppendChild(itemObj);
		Q_EMIT layoutChanged();
	}
}
