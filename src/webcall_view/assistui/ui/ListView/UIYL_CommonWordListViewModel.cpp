#include "UIYL_CommonWordListViewModel.h"
#include "utils/utilshelper.h"
#include <QToolTip>

UIYL_CommonWordListViewModel::UIYL_CommonWordListViewModel(QStringList headers, QObject *parent) : QAbstractItemModel(parent)
{
    m_headers  = headers;
    m_rootItem = new YL_TreeViewItem_CommonWord;
}

UIYL_CommonWordListViewModel::~UIYL_CommonWordListViewModel()
{
    if (m_rootItem != nullptr)
    {
        delete m_rootItem;
        m_rootItem = nullptr;
    }
}

YL_TreeViewItem_CommonWord *UIYL_CommonWordListViewModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
    {
        YL_TreeViewItem_CommonWord *item = static_cast<YL_TreeViewItem_CommonWord *>(index.internalPointer());
        return item;
    }
    return nullptr;
}

YL_TreeViewItem_CommonWord *UIYL_CommonWordListViewModel::root()
{
    return m_rootItem;
}

QVariant UIYL_CommonWordListViewModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant UIYL_CommonWordListViewModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        YL_TreeViewItem_CommonWord *item = static_cast<YL_TreeViewItem_CommonWord *>(index.internalPointer());
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
                    strData = strData.left(20).append("...");
                }

                if (item->IsLeaf())
                {
                    YL_TreeViewItemLeafNodeData_CommonWord *itemData  = (YL_TreeViewItemLeafNodeData_CommonWord *)item->Ptr();
                    QString                                 strHotKey = itemData->HotKey();
                    if (strHotKey != "" && !strHotKey.isEmpty())
                    {
                        strData += "(";
                        strData += strHotKey;
                        strData += ")";
                    }
                }

                QToolTip::showText(QCursor::pos(), strData);
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
                color = QColor("#f5f7fa");
            }
            else if (item->NodeState() == Enum_YL_NodeState_Select)
            {
                color = QColor("#e6e9ef");
            }

            return color;
        }
    }
    return QVariant();
}

Qt::ItemFlags UIYL_CommonWordListViewModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags               = flags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    return flags;
}

QModelIndex UIYL_CommonWordListViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    YL_TreeViewItem_CommonWord *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<YL_TreeViewItem_CommonWord *>(parent.internalPointer());

    if (parentItem)
    {
        YL_TreeViewItem_CommonWord *childItem = parentItem->Child(row);
        if (childItem)
        {
            return createIndex(row, column, childItem);
        }
    }
    return QModelIndex();
}

QModelIndex UIYL_CommonWordListViewModel::parent(const QModelIndex &index) const
{

    return QModelIndex();
}

int UIYL_CommonWordListViewModel::rowCount(const QModelIndex &parent) const
{
    YL_TreeViewItem_CommonWord *parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<YL_TreeViewItem_CommonWord *>(parent.internalPointer());

    if (parentItem)
    {
        return parentItem->ChildCount();
    }
    else
    {
        return 0;
    }
}

int UIYL_CommonWordListViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_headers.size();
}

void UIYL_CommonWordListViewModel::ClearAll()
{
    Q_EMIT layoutAboutToBeChanged();
    m_rootItem->RemoveChilds();
    Q_EMIT layoutChanged();
}

void UIYL_CommonWordListViewModel::AddCommonWord(MCommonWordData *objData)
{
    if (objData && m_rootItem)
    {
        YL_TreeViewItemLeafNodeData_CommonWord *itemData = new YL_TreeViewItemLeafNodeData_CommonWord();
        itemData->InitData(*objData, Enum_YL_NodeType_LeafNodePersonal);

        YL_TreeViewItem_CommonWord *itemObj = new YL_TreeViewItem_CommonWord(m_rootItem);
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
