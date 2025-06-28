#include "UISkinInfo.h"
#include "QMap"

UISkinInfo::UISkinInfo(void)
{
}

UISkinInfo::UISkinInfo(const UISkinInfo& info)
{
    this->Key(info.Key());
    this->FileName(info.FileName());
    this->Name(info.Name());
    this->GroupId(info.GroupId());
    this->GroupName(info.GroupName());
}

UISkinInfo::UISkinInfo(UISkinInfo* pinfo)
{
    if (pinfo)
    {
        this->Key(pinfo->Key());
        this->FileName(pinfo->FileName());
        this->Name(pinfo->Name());
        this->GroupId(pinfo->GroupId());
        this->GroupName(pinfo->GroupName());
    }
}

UISkinInfo::~UISkinInfo(void)
{
}

void UISkinInfo::FileName(QString val)
{
    m_filename = val;
}

QString UISkinInfo::FileName() const
{
    return m_filename;
}

void UISkinInfo::Key(QString val)
{
    m_key = val;
}

QString UISkinInfo::Key() const
{
    return m_key;
}

void UISkinInfo::Name(QString val)
{
    m_name = val;
}

QString UISkinInfo::Name() const
{
    return m_name;
}

int UISkinInfo::GroupId() const
{
    return m_groupId;
}
QString UISkinInfo::GroupName() const
{
    return m_groupName;
}

void UISkinInfo::GroupId(int gid)
{
    m_groupId = gid;
}

void UISkinInfo::GroupName(QString name)
{
    m_groupName = name;
}

void UISkinInfo::LoadMapInfo(const QMap<QString, QString>& mapInfo, bool ch)
{
    for (auto itor = mapInfo.begin(); itor != mapInfo.end(); itor++)
    {

        if (itor.key() == "groupid")
        {
            GroupId(itor.value().toInt());
        }
        else if (itor.key() == "groupname" && ch)
        {
            GroupName(itor.value());
        }
        else if (itor.key() == "groupname_en" && !ch)
        {
            GroupName(itor.value());
        }
        else if (itor.key() == "key")
        {
            Key(itor.value());
        }
        else if (itor.key() == "fileName")
        {
            FileName(itor.value());
        }
        else if (itor.key() == "name")
        {
            Name(itor.value());
        }
    }
}

QMap<QString, QString> UISkinInfo::InfoMap() const
{
    QMap<QString, QString> mapInfo;

    mapInfo.insert("groupid", QString("%1").arg(GroupId()));

    mapInfo.insert("groupname", QString("%1").arg(GroupName()));

    mapInfo.insert("key", QString("%1").arg(Key()));

    mapInfo.insert("fileName", QString("%1").arg(FileName()));

    mapInfo.insert("name", QString("%1").arg(Name()));

    return mapInfo;
}
