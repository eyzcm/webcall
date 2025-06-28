#include "UIVisitorCol.h"

UIVisitorCol::UIVisitorCol(void)
{
    m_sortIndex = 0;
}

UIVisitorCol::UIVisitorCol(const UIVisitorCol& info)
{
    this->ColName(info.ColName());
    this->ColType(info.ColType());
    this->Hidden(info.Hidden());
    this->Id(info.Id());
    this->SelfText(info.SelfText());
    this->Itemtype(info.Itemtype());
    this->Text(info.Text());
    this->SortIndex(info.SortIndex());
    Q_FOREACH (CommInfo* obj, info.ItemList())
    {
        if (obj)
        {
            CommInfo* tempObj = new CommInfo(*obj);
            m_itemList.push_back(tempObj);
        }
    }
}

UIVisitorCol::UIVisitorCol(UIVisitorCol* info)
{
    if (info == nullptr)
        return;
    this->ColName(info->ColName());
    this->ColType(info->ColType());
    this->Hidden(info->Hidden());
    this->Id(info->Id());
    this->SelfText(info->SelfText());
    this->Itemtype(info->Itemtype());
    this->Text(info->Text());
    this->SortIndex(info->SortIndex());
    Q_FOREACH (CommInfo* obj, info->ItemList())
    {
        if (obj)
        {
            CommInfo* tempObj = new CommInfo(*obj);
            m_itemList.push_back(tempObj);
        }
    }
}

UIVisitorCol::~UIVisitorCol(void)
{
    qDeleteAll(m_itemList);
    m_itemList.clear();
}

void UIVisitorCol::ColType(QString val)
{
    m_colType = val;
}

QString UIVisitorCol::ColType() const
{
    return m_colType;
}

void UIVisitorCol::Hidden(bool val)
{
    m_isHidden = val;
}

bool UIVisitorCol::Hidden() const
{
    return m_isHidden;
}

void UIVisitorCol::Id(QString val)
{
    m_id = val;
}

QString UIVisitorCol::Id() const
{
    return m_id;
}

void UIVisitorCol::ColName(QString val)
{
    m_colName = val;
}

QString UIVisitorCol::ColName() const
{
    return m_colName;
}

void UIVisitorCol::SelfText(QString val)
{
    m_selfText = val;
}

QString UIVisitorCol::SelfText() const
{
    return m_selfText;
}

void UIVisitorCol::Itemtype(QString val)
{
    m_Itemtype = val;
}

QString UIVisitorCol::Itemtype() const
{
    return m_Itemtype;
}

void UIVisitorCol::Text(QString val)
{
    m_text = val;
}

QString UIVisitorCol::Text() const
{
    return m_text;
}

QList<CommInfo*> UIVisitorCol::ItemList() const
{
    return m_itemList;
}

void UIVisitorCol::ItemList(QList<CommInfo*> val)
{
    m_itemList = val;
}

void UIVisitorCol::AddItem(const CommInfo& info)
{
    CommInfo* item = new CommInfo(info);
    m_itemList.append(item);
}

int UIVisitorCol::SortIndex() const
{
    return m_sortIndex;
}

void UIVisitorCol::SortIndex(int val)
{
    m_sortIndex = val;
}
