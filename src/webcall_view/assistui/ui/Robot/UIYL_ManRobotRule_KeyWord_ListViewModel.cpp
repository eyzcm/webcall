#include "UIYL_ManRobotRule_KeyWord_ListViewModel.h"

UIYL_ManRobotRule_KeyWord_ListViewModel::UIYL_ManRobotRule_KeyWord_ListViewModel(QStringList headers, QObject* parent) : QAbstractItemModel(parent)
{
    m_headers  = headers;
    m_rootItem = new YL_ManRobotRuleListItem();
}

UIYL_ManRobotRule_KeyWord_ListViewModel::~UIYL_ManRobotRule_KeyWord_ListViewModel()
{
    delete m_rootItem;
}

QVariant UIYL_ManRobotRule_KeyWord_ListViewModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags UIYL_ManRobotRule_KeyWord_ListViewModel::flags(const QModelIndex& index) const
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

QModelIndex UIYL_ManRobotRule_KeyWord_ListViewModel::index(int row, int column, const QModelIndex& parent) const
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

QModelIndex UIYL_ManRobotRule_KeyWord_ListViewModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

int UIYL_ManRobotRule_KeyWord_ListViewModel::rowCount(const QModelIndex& parent) const
{
    YL_ManRobotRuleListItem* parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<YL_ManRobotRuleListItem*>(parent.internalPointer());

    return parentItem->ChildCount();
}

int UIYL_ManRobotRule_KeyWord_ListViewModel::columnCount(const QModelIndex& parent) const
{
    return m_headers.size();
}

YL_ManRobotRuleListItem* UIYL_ManRobotRule_KeyWord_ListViewModel::root()
{
    return m_rootItem;
}

YL_ManRobotRuleListItem* UIYL_ManRobotRule_KeyWord_ListViewModel::itemFromIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return NULL;

    YL_ManRobotRuleListItem* item = static_cast<YL_ManRobotRuleListItem*>(index.internalPointer());
    return item;
}

QVariant UIYL_ManRobotRule_KeyWord_ListViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    YL_ManRobotRuleListItem* item = static_cast<YL_ManRobotRuleListItem*>(index.internalPointer());
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

    return QVariant();
}

bool UIYL_ManRobotRule_KeyWord_ListViewModel::removeRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
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
            RemoveCache_RuleItem_ById(id);
            m_rootItem->RemoveChildByRow(slnRemove);
        }
    }
    endRemoveRows();
    return true;
}

bool UIYL_ManRobotRule_KeyWord_ListViewModel::FindListItemById(QString id)
{
    bool ret  = false;
    auto itor = m_mapRuleItemList.find(id);
    if (itor != m_mapRuleItemList.end())
    {
        ret = true;
    }
    return ret;
}

bool UIYL_ManRobotRule_KeyWord_ListViewModel::FindListLeafNodeItemDataById(QString id)
{
    bool ret  = false;
    auto itor = m_mapRuleLeafNodeItemData.find(id);
    if (itor != m_mapRuleLeafNodeItemData.end())
    {
        ret = true;
    }
    return ret;
}

QStringList UIYL_ManRobotRule_KeyWord_ListViewModel::GetIdList()
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

YL_ManRobotRuleListItem* UIYL_ManRobotRule_KeyWord_ListViewModel::GetListItemById(QString id)
{
    YL_ManRobotRuleListItem* ret  = nullptr;
    auto                     itor = m_mapRuleItemList.find(id);
    if (itor != m_mapRuleItemList.end())
    {
        ret = itor.value();
    }
    return ret;
}

YL_ManRobotRuleLeafNodeItemData* UIYL_ManRobotRule_KeyWord_ListViewModel::GetListLeafNodeItemDataById(QString id)
{
    YL_ManRobotRuleLeafNodeItemData* ret  = nullptr;
    auto                             itor = m_mapRuleLeafNodeItemData.find(id);
    if (itor != m_mapRuleLeafNodeItemData.end())
    {
        ret = itor.value();
    }
    return ret;
}

void UIYL_ManRobotRule_KeyWord_ListViewModel::Add_RuleItem(MUIManRobotRuleData* objData)
{
    if (objData)
    {
        if (m_rootItem)
        {
            YL_ManRobotRuleLeafNodeItemData* itemObj = new YL_ManRobotRuleLeafNodeItemData();
            itemObj->InitData(*objData);
            m_mapRuleLeafNodeItemData.insert(itemObj->Id(), itemObj);

            YL_ManRobotRuleListItem* listItemObj = new YL_ManRobotRuleListItem(m_rootItem);
            listItemObj->SetLeaf(true);
            listItemObj->SetId(itemObj->Id());
            listItemObj->SetPtr(itemObj);

            int insertPos = m_rootItem->InsertChildPostion(listItemObj);

            beginInsertRows(QModelIndex(), insertPos, insertPos);
            m_mapRuleItemList.insert(listItemObj->Id(), listItemObj);
            m_rootItem->InsertChild(listItemObj, insertPos);
            endInsertRows();
        }
    }
}

void UIYL_ManRobotRule_KeyWord_ListViewModel::Move_RuleItem(YL_ManRobotRuleListItem* curItem, YL_ManRobotRuleListItem* hoverItem)
{
    if (curItem == nullptr || hoverItem == nullptr)
    {
        return;
    }

    YL_ManRobotRuleLeafNodeItemData* itemObj  = GetListLeafNodeItemDataById(curItem->Id());
    YL_ManRobotRuleLeafNodeItemData* hoverObj = GetListLeafNodeItemDataById(hoverItem->Id());
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
                    YL_ManRobotRuleLeafNodeItemData* tempItemObj = GetListLeafNodeItemDataById(tempItem->Id());
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
                    YL_ManRobotRuleLeafNodeItemData* tempItemObj = GetListLeafNodeItemDataById(tempItem->Id());
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

void UIYL_ManRobotRule_KeyWord_ListViewModel::Delete_RuleItem(QString id)
{
    if (m_rootItem)
    {
        int nRow = m_rootItem->FindChildPostionById(id);
        if (nRow == -1)
        {
            RemoveCache_RuleItem_ById(id);
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

void UIYL_ManRobotRule_KeyWord_ListViewModel::Clear_RuleItem()
{
    Q_EMIT layoutAboutToBeChanged();
    m_rootItem->RemoveChilds();
    m_mapRuleItemList.clear();
    m_mapRuleLeafNodeItemData.clear();
    Q_EMIT layoutChanged();
}

void UIYL_ManRobotRule_KeyWord_ListViewModel::RemoveCache_RuleItem_ById(QString id)
{
    m_mapRuleItemList.remove(id);
    m_mapRuleLeafNodeItemData.remove(id);
}
