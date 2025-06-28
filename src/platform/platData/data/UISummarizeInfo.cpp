#include "UISummarizeInfo.h"

UISummarizeInfo::UISummarizeInfo(void)
{
}

UISummarizeInfo::UISummarizeInfo(const UISummarizeInfo& info)
{
    foreach (UISummarizeInfo* member, info.ChildList())
    {
        if (member)
        {
            UISummarizeInfo* obj = new UISummarizeInfo(*member);
            m_childList.push_back(obj);
        }
    }
    this->Content(info.Content());
    this->Id(info.Id());
    this->ParentId(info.ParentId());
    this->Title(info.Title());
    this->Type(info.Type());
    this->Color(info.Color());
    this->SortIndex(info.SortIndex());
    this->categorySource(info.categorySource());
}
UISummarizeInfo::UISummarizeInfo(UISummarizeInfo* info)
{
    if (info == nullptr)
        return;

    if (info->ChildList().count() > 0)
    {
        foreach (UISummarizeInfo* member, info->ChildList())
        {
            if (member)
            {
                UISummarizeInfo* obj = new UISummarizeInfo(member);
                m_childList.push_back(obj);
            }
        }
    }
    this->Content(info->Content());
    this->Id(info->Id());
    this->ParentId(info->ParentId());
    this->Title(info->Title());
    this->Type(info->Type());
    this->Color(info->Color());
    this->SortIndex(info->SortIndex());
    this->categorySource(info->categorySource());
}

void UISummarizeInfo::Id(QString val)
{
    m_id = val;
}

QString UISummarizeInfo::Id() const
{
    return m_id;
}

void UISummarizeInfo::ParentId(QString val)
{
    m_parentId = val;
}

QString UISummarizeInfo::ParentId() const
{
    return m_parentId;
}

void UISummarizeInfo::Content(QString val)
{
    m_content = val;
}

QString UISummarizeInfo::Content() const
{
    return m_content;
}

void UISummarizeInfo::Title(QString val)
{
    m_title = val;
}

QString UISummarizeInfo::Title() const
{
    return m_title;
}

void UISummarizeInfo::ChildList(QList<UISummarizeInfo*> val)
{
    m_childList = val;
}

QList<UISummarizeInfo*> UISummarizeInfo::ChildList() const
{
    return m_childList;
}

void UISummarizeInfo::AddChild(const UISummarizeInfo& info)
{
    UISummarizeInfo* objInfo = new UISummarizeInfo(info);
    m_childList.append(objInfo);
}


void UISummarizeInfo::AddChild(UISummarizeInfo* info)
{
    m_childList.append(info);
}


UISummarizeInfo::~UISummarizeInfo(void)
{
    qDeleteAll(m_childList);
    m_childList.clear();
}


QString UISummarizeInfo::categorySource() const
{
    return m_categorySource;
}

void UISummarizeInfo::categorySource(QString val)
{
    m_categorySource = val;
}


void UISummarizeInfo::Color(QString val)
{
    m_color = val;
}

QString UISummarizeInfo::Color() const
{
    return m_color;
}

void UISummarizeInfo::SortIndex(QString val)
{
    m_sortIndex = val;
}

QString UISummarizeInfo::SortIndex() const
{
    return m_sortIndex;
}

void UISummarizeInfo::Type(QString val)
{
    m_type = val;
}

QString UISummarizeInfo::Type() const
{
    return m_type;
}
