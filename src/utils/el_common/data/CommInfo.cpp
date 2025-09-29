#include "CommInfo.h"

#include <utils/jsonhelper.h>


CommInfo CommInfo::NullInfo;
CommInfo::CommInfo(void)
{
}

CommInfo::~CommInfo(void)
{
    m_infoMap.clear();
}

CommInfo::CommInfo(const CommInfo &o)
{
    this->m_infoMap = o.m_infoMap;
}

CommInfo::CommInfo(CommInfo *info)
{
    this->m_infoMap = info->m_infoMap;
}

void CommInfo::AddCommInfo(const QMap<QString, QString> &mapInfo)
{
    for (auto itor = mapInfo.begin(); itor != mapInfo.end(); itor++)
    {
        m_infoMap.insert(itor.key(), itor.value());
    }
}

int CommInfo::GetInfoSize() const
{
    return m_infoMap.size();
}

QString CommInfo::GetValueByKey(const QString &key) const
{
    QString ret = "";
    if (m_infoMap.find(key) != m_infoMap.end())
    {
        ret = m_infoMap[key];
    }
    return ret;
}

QString CommInfo::GetValueByKeyHump(const QString &key) const
{
    QString newKey = JsonHelper::ChangeToHump(key);
    QString ret    = "";
    if (m_infoMap.find(newKey) != m_infoMap.end())
    {
        ret = m_infoMap[newKey];
    }
    else if (newKey != key)
    {
        if (m_infoMap.find(key) != m_infoMap.end())
        {
            ret = m_infoMap[key];
        }
    }

    return ret;
}
void CommInfo::SetValueByKey(const QString &key, const QString &val)
{
    m_infoMap[key] = val;
}


// CZC:判断是否存在 key
bool CommInfo::HasKey(QString key)
{

    if (m_infoMap.contains(key))
    {
        return true;
    }
    return false;
}
