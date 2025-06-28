#include "UIMenuDataInfo.h"


UIMenuDataInfo::UIMenuDataInfo(void)
{
    m_id       = 0;
    m_parentId = 0;
    m_score    = 0;
    m_opentype = -1;
    m_flag     = 0;
}

UIMenuDataInfo::~UIMenuDataInfo(void)
{
}

UIMenuDataInfo::UIMenuDataInfo(UIMenuDataInfo *info)
{
    m_id       = 0;
    m_parentId = 0;
    m_score    = 0;
    m_opentype = -1;
    if (info)
    {
        Id(info->Id());
        Name(info->Name());
        Code(info->Code());
        Url(info->Url());
        Score(info->Score());
        IconUrl(info->IconUrl());
        m_iconFile = info->IconFile();
        IconUrlClicked(info->IconUrlClicked());
        ParentId(info->ParentId());
        Flag(info->Flag());
        Opentype(info->Opentype());
    }
}

QString UIMenuDataInfo::Name() const
{
    return m_name;
}

void UIMenuDataInfo::Name(QString val)
{
    m_name = val;
}

QString UIMenuDataInfo::Code() const
{
    return m_code;
}

void UIMenuDataInfo::Code(QString val)
{
    m_code = val;
}

QString UIMenuDataInfo::Url() const
{
    return m_url;
}

void UIMenuDataInfo::Url(QString val)
{
    m_url = val;
}

int UIMenuDataInfo::Score() const
{
    return m_score;
}

void UIMenuDataInfo::Score(int val)
{
    m_score = val;
}

QString UIMenuDataInfo::IconFile() const
{
    return m_iconFile;
}

void UIMenuDataInfo::IconFile(QString val)
{
    m_iconFile = val;
}

QString UIMenuDataInfo::IconUrl() const
{
    return m_iconUrl;
}

void UIMenuDataInfo::IconUrl(QString val)
{
    m_iconUrl = val;
}

QString UIMenuDataInfo::IconUrlClicked() const
{
    return m_iconUrlClicked;
}

void UIMenuDataInfo::IconUrlClicked(QString val)
{
    m_iconUrlClicked = val;
}

int UIMenuDataInfo::ParentId() const
{
    return m_parentId;
}

void UIMenuDataInfo::ParentId(int val)
{
    m_parentId = val;
}

int UIMenuDataInfo::Flag() const
{
    return m_flag;
}

void UIMenuDataInfo::Flag(int val)
{
    m_flag = val;
}

int UIMenuDataInfo::Opentype() const
{
    return m_opentype;
}

void UIMenuDataInfo::Opentype(int val)
{
    m_opentype = val;
}

void UIMenuDataInfo::LoadMapInfo(const QMap<QString, QString> &mapInfo)
{
    for (auto itor = mapInfo.begin(); itor != mapInfo.end(); itor++)
    {
        if (itor.key() == "name")
        {
            Name(itor.value());
        }
        else if (itor.key() == "code")
        {
            Code(itor.value());
		}
		else if (itor.key() == "subName")
		{
			SubName(itor.value());
        }
        else if (itor.key() == "url")
        {
			if( Url().isEmpty() )
				Url(itor.value());
		}
		else if (itor.key() == "linkUrl")
		{
			Url(itor.value());
        }
        else if (itor.key() == "score")
        {
            Score(itor.value().toInt());
        }
        else if (itor.key() == "iconUrl")
        {
            IconUrl(itor.value());
        }
        else if (itor.key() == "iconUrlClicked")
        {
            IconUrlClicked(itor.value());
        }
        else if (itor.key() == "parentId")
        {
            ParentId(itor.value().toInt());
        }
        else if (itor.key() == "id")
        {
            Id(itor.value().toInt());
        }
        else if (itor.key() == "flag")
        {
            Flag(itor.value().toInt());
        }
        else if (itor.key() == "opentype")
        {
            Opentype(itor.value().toInt());
        }
    }
    if (Opentype() == -1)
    {
        //默认设置打开方式

    }
}

QString UIMenuDataInfo::SubName() const
{
	return m_subName;
}

void    UIMenuDataInfo::SubName(QString val)
{
	m_subName = val;
}

QMap<QString, QString> UIMenuDataInfo::InfoMap() const
{
    QMap<QString, QString> mapInfo;
    mapInfo.insert("id", QString("%1").arg(Id()));
    mapInfo.insert("name", QString("%1").arg(Name()));
	mapInfo.insert("subName", QString("%1").arg(SubName()));
    mapInfo.insert("code", QString("%1").arg(Code()));
    mapInfo.insert("url", QString("%1").arg(Url()));
    mapInfo.insert("score", QString("%1").arg(Score()));
    mapInfo.insert("iconUrl", QString("%1").arg(IconUrl()));
    mapInfo.insert("iconUrlClicked", QString("%1").arg(IconUrlClicked()));
    mapInfo.insert("parentId", QString("%1").arg(ParentId()));
    mapInfo.insert("flag", QString("%1").arg(Flag()));
    mapInfo.insert("opentype", QString("%1").arg(Opentype()));
    return mapInfo;
}
