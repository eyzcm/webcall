#include "UI_PersonalQuickCommonWord_ListViewItemModel.h"
#include "data\YLData\YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord.h"
#include "data\YLData\YL_TreeViewItemLeafNodeData_CommonWord.h"
#include "utils/utilshelper.h"
#include <QToolTip>

UI_PersonalQuickCommonWord_ListViewItemModel::UI_PersonalQuickCommonWord_ListViewItemModel(QStringList headers, QObject *parent) : QAbstractItemModel(parent)
{
    m_headers  = headers;
    m_rootItem = new YL_PersonalQuickTreeViewItem_CommonWord;
}

UI_PersonalQuickCommonWord_ListViewItemModel::~UI_PersonalQuickCommonWord_ListViewItemModel()
{
    if (m_rootItem != nullptr)
    {
        delete m_rootItem;
        m_rootItem = nullptr;
    }
}

YL_PersonalQuickTreeViewItem_CommonWord *UI_PersonalQuickCommonWord_ListViewItemModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
    {
        YL_PersonalQuickTreeViewItem_CommonWord *item = static_cast<YL_PersonalQuickTreeViewItem_CommonWord *>(index.internalPointer());
        return item;
    }
    return nullptr;
}

YL_PersonalQuickTreeViewItem_CommonWord *UI_PersonalQuickCommonWord_ListViewItemModel::root()
{
    return m_rootItem;
}

QVariant UI_PersonalQuickCommonWord_ListViewItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return m_headers.at(section);
        }
    }
    return QVariant();
}

QVariant UI_PersonalQuickCommonWord_ListViewItemModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        YL_PersonalQuickTreeViewItem_CommonWord *item = static_cast<YL_PersonalQuickTreeViewItem_CommonWord *>(index.internalPointer());
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
                if (strData.length() > 20)
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

Qt::ItemFlags UI_PersonalQuickCommonWord_ListViewItemModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
    {
        QAbstractItemModel::flags(index);
    }
    return 0;
}

QModelIndex UI_PersonalQuickCommonWord_ListViewItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    YL_PersonalQuickTreeViewItem_CommonWord *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<YL_PersonalQuickTreeViewItem_CommonWord *>(parent.internalPointer());

    YL_PersonalQuickTreeViewItem_CommonWord *childItem = parentItem->Child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
        return QModelIndex();
}

QModelIndex UI_PersonalQuickCommonWord_ListViewItemModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int UI_PersonalQuickCommonWord_ListViewItemModel::rowCount(const QModelIndex &parent) const
{
    YL_PersonalQuickTreeViewItem_CommonWord *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<YL_PersonalQuickTreeViewItem_CommonWord *>(parent.internalPointer());

    return parentItem->ChildCount();
}

int UI_PersonalQuickCommonWord_ListViewItemModel::columnCount(const QModelIndex &parent) const
{
    return m_headers.size();
}

void UI_PersonalQuickCommonWord_ListViewItemModel::ClearAll()
{
    Q_EMIT layoutAboutToBeChanged();
    m_rootItem->RemoveChilds();
    Q_EMIT layoutChanged();
}

void UI_PersonalQuickCommonWord_ListViewItemModel::AddPersonalQuickCommonWord(MPersonalQuickCommonWordData *objData)
{
    if (objData && m_rootItem)
    {
        YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord *itemData = new YL_PersonalQuickTreeViewItemLeafNodeData_CommonWord();
        itemData->InitData(*objData);

        YL_PersonalQuickTreeViewItem_CommonWord *itemObj = new YL_PersonalQuickTreeViewItem_CommonWord(m_rootItem);
        itemObj->SetLeaf(true);
        itemObj->SetId(itemData->Id());
        itemObj->SetPtr(itemData);

        Q_EMIT layoutAboutToBeChanged();
        m_rootItem->AppendChild(itemObj);
        Q_EMIT layoutChanged();
    }
}

void UI_PersonalQuickCommonWord_ListViewItemModel::RemoveItemById(QString id)
{
    Q_EMIT layoutAboutToBeChanged();
    m_rootItem->RemoveChildById(id);
    Q_EMIT layoutChanged();
}
