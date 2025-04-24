#include "UIYL_ManRobotRuleListViewModel.h"
#include "utils/utilshelper.h"
#include <QColor>
#include <QDateTime>
#include <QStringList>
#include <QToolTip>

UIYL_ManRobotRuleListViewModel::UIYL_ManRobotRuleListViewModel(QStringList headers, QObject* parent) : QAbstractItemModel(parent)
{
    m_headers  = headers;
    m_rootItem = new YL_ManRobotRuleListItem;
}

UIYL_ManRobotRuleListViewModel::~UIYL_ManRobotRuleListViewModel()
{
    // delete m_rootItem;
    // m_rootItem = nullptr;
}

YL_ManRobotRuleListItem* UIYL_ManRobotRuleListViewModel::itemFromIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return NULL;
    YL_ManRobotRuleListItem* item = static_cast<YL_ManRobotRuleListItem*>(index.internalPointer());
    return item;
}

YL_ManRobotRuleListItem* UIYL_ManRobotRuleListViewModel::root()
{
    return m_rootItem;
}

QVariant UIYL_ManRobotRuleListViewModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant UIYL_ManRobotRuleListViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    YL_ManRobotRuleListItem* item = static_cast<YL_ManRobotRuleListItem*>(index.internalPointer());
    if (role == Qt::DisplayRole)
    {
        return QVariant();
        // return item->Data();
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
            color = QColor("#f5f7fa");
        }
        else if (item->NodeState() == Enum_YL_NodeState_Select)
        {
            color = QColor("#e6e9ef");
        }

        return color;
    }

    return QVariant();
}

Qt::ItemFlags UIYL_ManRobotRuleListViewModel::flags(const QModelIndex& index) const
{
    // if (!index.isValid())
    //{
    //	return 0;
    // }
    // return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags               = flags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    return flags;
}

QModelIndex UIYL_ManRobotRuleListViewModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    YL_ManRobotRuleListItem* parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<YL_ManRobotRuleListItem*>(parent.internalPointer());

    YL_ManRobotRuleListItem* childItem = parentItem->Child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
        return QModelIndex();
}

QModelIndex UIYL_ManRobotRuleListViewModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

int UIYL_ManRobotRuleListViewModel::rowCount(const QModelIndex& parent) const
{
    YL_ManRobotRuleListItem* parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<YL_ManRobotRuleListItem*>(parent.internalPointer());

    return parentItem->ChildCount();
}

int UIYL_ManRobotRuleListViewModel::columnCount(const QModelIndex& parent) const
{
    return m_headers.size();
}

YL_ManRobotRuleLeafNodeItemData* UIYL_ManRobotRuleListViewModel::GetMainTreeLeafNodeItemDataById(QString id)
{
    YL_ManRobotRuleLeafNodeItemData* ret  = nullptr;
    auto                             itor = m_manRobotRuleLeafNodeItemDataList.find(id);
    if (itor != m_manRobotRuleLeafNodeItemDataList.end())
    {
        ret = itor.value();
    }
    return ret;
}

void UIYL_ManRobotRuleListViewModel::RemoveCacheItemById(QString id)
{
    m_manRobotRuleItemList.remove(id);
    m_manRobotRuleLeafNodeItemDataList.remove(id);
}

YL_ManRobotRuleListItem* UIYL_ManRobotRuleListViewModel::GetMainTreeItemById(QString id)
{
    YL_ManRobotRuleListItem* ret  = nullptr;
    auto                     itor = m_manRobotRuleItemList.find(id);
    if (itor != m_manRobotRuleItemList.end())
    {
        ret = itor.value();
    }
    return ret;
}

bool UIYL_ManRobotRuleListViewModel::FindMainTreeItemById(QString id)
{
    bool ret  = false;
    auto itor = m_manRobotRuleItemList.find(id);
    if (itor != m_manRobotRuleItemList.end())
    {
        ret = true;
    }
    return ret;
}

void UIYL_ManRobotRuleListViewModel::ClearAll()
{
    Q_EMIT layoutAboutToBeChanged();
    m_rootItem->RemoveChilds();
    // to do shizhenyu
    m_manRobotRuleItemList.clear();
    m_manRobotRuleLeafNodeItemDataList.clear();
    Q_EMIT layoutChanged();
}

void UIYL_ManRobotRuleListViewModel::AddManRobotRuleData(MUIManRobotRuleData* objData)
{
    if (objData)
    {
        if (m_rootItem)
        {
            YL_ManRobotRuleLeafNodeItemData* itemObj = new YL_ManRobotRuleLeafNodeItemData();
            itemObj->InitData(*objData);
            m_manRobotRuleLeafNodeItemDataList.insert(itemObj->Id(), itemObj);

            YL_ManRobotRuleListItem* customerTreeItemObj = new YL_ManRobotRuleListItem(m_rootItem);
            customerTreeItemObj->SetLeaf(true);
            customerTreeItemObj->SetId(itemObj->Id());
            customerTreeItemObj->SetPtr(itemObj);

            int insertPos = m_rootItem->InsertChildPostion(customerTreeItemObj);

            beginInsertRows(QModelIndex(), insertPos, insertPos);
            m_manRobotRuleItemList.insert(customerTreeItemObj->Id(), customerTreeItemObj);
            m_rootItem->InsertChild(customerTreeItemObj, insertPos);
            endInsertRows();
        }
    }
}

bool UIYL_ManRobotRuleListViewModel::FindCustomerTreeLeafNodeItemDataById(QString id)
{
    bool ret  = false;
    auto itor = m_manRobotRuleLeafNodeItemDataList.find(id);
    if (itor != m_manRobotRuleLeafNodeItemDataList.end())
    {
        ret = true;
    }
    return ret;
}

void UIYL_ManRobotRuleListViewModel::MoveItem(YL_ManRobotRuleListItem* curItem, YL_ManRobotRuleListItem* hoverItem)
{
    if (curItem == nullptr || hoverItem == nullptr)
    {
        return;
    }
    YL_ManRobotRuleLeafNodeItemData* itemObj  = GetMainTreeLeafNodeItemDataById(curItem->Id());
    YL_ManRobotRuleLeafNodeItemData* hoverObj = GetMainTreeLeafNodeItemDataById(hoverItem->Id());

    if (itemObj && hoverObj)
    {

        int curIndex   = curItem->Row();
        int hoverIndex = hoverItem->Row();
        if (curIndex > hoverIndex)
        {
            int tempRank = itemObj->Rank();
            for (int i = curIndex - 1; i > hoverIndex; i--)
            {
                YL_ManRobotRuleListItem* tempItem = m_rootItem->Child(i);

                if (tempItem)
                {
                    YL_ManRobotRuleLeafNodeItemData* tempItemObj = GetMainTreeLeafNodeItemDataById(tempItem->Id());
                    if (tempItemObj)
                    {
                        int valRank = tempItemObj->Rank();
                        tempItemObj->Rank(tempRank);
                        tempRank = valRank;
                    }
                }
            }
            itemObj->Rank(tempRank);
        }
        else if (curIndex == hoverIndex)
        {
            Q_ASSERT(false);
        }
        else
        {
            int tempRank = itemObj->Rank();
            for (int i = curIndex + 1; i < hoverIndex + 1; i++)
            {
                YL_ManRobotRuleListItem* tempItem = m_rootItem->Child(i);

                if (tempItem)
                {
                    YL_ManRobotRuleLeafNodeItemData* tempItemObj = GetMainTreeLeafNodeItemDataById(tempItem->Id());
                    if (tempItemObj)
                    {
                        int valRank = tempItemObj->Rank();
                        tempItemObj->Rank(tempRank);
                        tempRank = valRank;
                    }
                }
            }
            itemObj->Rank(tempRank);
        }

        Q_EMIT layoutAboutToBeChanged();
        m_rootItem->UpdateChild(curItem);
        Q_EMIT layoutChanged();
    }
}

bool UIYL_ManRobotRuleListViewModel::removeRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
{
    Q_ASSERT(count > 0);
    Q_ASSERT(row >= 0);
    Q_ASSERT(rowCount() >= (row + count - 1));

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    int slnRemove = row;
    for (int r = row; r < (row + count); ++r)
    {
        YL_ManRobotRuleListItem* removeItem = m_rootItem->Child(slnRemove);
        if (removeItem)
        {
            QString id = removeItem->Id();
            RemoveCacheItemById(id);
            m_rootItem->RemoveChildByRow(slnRemove);
        }
    }
    endRemoveRows();
    return true;
}

void UIYL_ManRobotRuleListViewModel::DeleteManRobotRuleItem(QString id)
{
    if (m_rootItem)
    {
        int nRow = m_rootItem->FindChildPostionById(id);

        if (nRow == -1)
        {
            RemoveCacheItemById(id);
        }
        else
        {
            removeRows(nRow, 1);
        }
    }
    else
    {
        Q_ASSERT(false);
    }
}

QStringList UIYL_ManRobotRuleListViewModel::GetIdList()
{
    QStringList retList;
    if (m_rootItem)
    {
        QList<YL_ManRobotRuleListItem*> itemList = m_rootItem->ChildItems();
        Q_FOREACH (YL_ManRobotRuleListItem* item, itemList)
        {
            if (item)
            {
                retList.push_back(item->Id());
            }
        }
    }
    return retList;
}
