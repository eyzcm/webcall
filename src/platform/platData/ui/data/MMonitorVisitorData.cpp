#include "MMonitorVisitorData.h"
 #include "utils/DefineHelper.h"
//#include "service/DbUiwebcallService.h"

MMonitorVisitorData::MMonitorVisitorData(void)
{
}

MMonitorVisitorData::MMonitorVisitorData(const MMonitorVisitorData& o)
{
    Vid(o.Vid());
    Userid(o.Userid());
    Name(o.Name());
    VisitorStatus(o.VisitorStatus());
    VisitorInfoMap(o.VisitorInfoMap());
    ExtendInfo(o.ExtendInfo());
}

MMonitorVisitorData::~MMonitorVisitorData(void)
{
}

void MMonitorVisitorData::Userid(QString val)
{
    m_userid = val;
}

QString MMonitorVisitorData::Userid() const
{
    return m_userid;
}

void MMonitorVisitorData::Vid(QString val)
{
    m_vid = val;
}

QString MMonitorVisitorData::Vid() const
{
    return m_vid;
}

void MMonitorVisitorData::Name(QString val)
{
    m_name = val;
}

QString MMonitorVisitorData::Name() const
{
    return m_name;
}

void MMonitorVisitorData::InitVisitorInfo(const UIVisitorInfo& info,QString name ,QString siteName )
{
    if (!info.Vid().isEmpty())
    { 
       
		Name(name);// gUICacheService->GetDisplayName(&info));
        Userid(info.StaticId());
        Vid(info.Vid());
        VisitorStatus(info.VisitorStatus());
        
        m_visitorInfoMap.insert("name", name);
        m_visitorInfoMap.insert("userId", info.StaticId());
        m_visitorInfoMap.insert("inviteChatId", info.StaticId());
        m_visitorInfoMap.insert("vid", info.Vid());
        m_visitorInfoMap.insert("createTime", info.CreateTime());
        m_visitorInfoMap.insert("visitorStatus", QString::number((int)info.VisitorStatus()));
        m_visitorInfoMap.insert("firstActiveUrl", info.FirstActiveUrl());
        m_visitorInfoMap.insert("chatUrl", info.ChatUrl());
        m_visitorInfoMap.insert("lastActiveUrl", info.LastActiveUrl());
        m_visitorInfoMap.insert("searchEngineId", info.SearchEngineId());
        m_visitorInfoMap.insert("searchEngineName", info.SearchEngineName());
        m_visitorInfoMap.insert("promotionId", info.PromotionId());
        m_visitorInfoMap.insert("promotionName", info.PromotionName());
        m_visitorInfoMap.insert("siteId", info.SiteId());
        
        m_visitorInfoMap.insert("siteName", siteName);

        m_visitorInfoMap.insert("keyWord", info.KeyWord());
        m_visitorInfoMap.insert("urlRefer", info.UrlRefer());
        m_visitorInfoMap.insert("device", info.Device());
        m_visitorInfoMap.insert("visitorCount", info.VisitorCount());
        m_visitorInfoMap.insert("ip", info.Ip());
        m_visitorInfoMap.insert("visitorStartTime", info.VisitorStartTime());
        m_visitorInfoMap.insert("visitorLocationCity", info.Visitor_location_city());
        m_visitorInfoMap.insert("visitorLocationProvince", info.Visitor_location_province());
        m_visitorInfoMap.insert("visitorLocationCountry", info.Visitor_location_country());
        m_visitorInfoMap.insert("visitorLocationDistrict", info.Visitor_location_district());
    }
}

QString MMonitorVisitorData::GetUIVisitorInfoValueByKey(QString strKey) const
{
    QString ret = "";
    if (m_visitorInfoMap.find(strKey) != m_visitorInfoMap.end())
    {
        ret = m_visitorInfoMap[strKey];
    }
    return ret;
}

void MMonitorVisitorData::SetUIVisitorInfoValueByKey(QString strKey, QString strVal)
{
    if (m_visitorInfoMap.find(strKey) != m_visitorInfoMap.end())
    {
        m_visitorInfoMap.insert(strKey, strVal);
    }
}

EVISITORSTATUS MMonitorVisitorData::VisitorStatus() const
{
    return m_visitorStatus;
}

void MMonitorVisitorData::VisitorStatus(EVISITORSTATUS val)
{
    m_visitorStatus = val;
}

void MMonitorVisitorData::VisitorInfoMap(QMap<QString, QString> val)
{
    m_visitorInfoMap = val;
}

QMap<QString, QString> MMonitorVisitorData::VisitorInfoMap() const
{
    return m_visitorInfoMap;
}

bool MMonitorVisitorData::IsFilter(QString searchKey, CommInfo filterInfo)
{
    bool isRet = true;
    do
    {
        if (!searchKey.isEmpty())
        {
            if (!Name().contains(searchKey) && !Userid().contains(searchKey) && !Vid().contains(searchKey))
            {
                isRet = false;
                break;
            }
        }
        QString visitorType  = filterInfo.GetValueByKey("visitorType");
        int     visitorCount = GetUIVisitorInfoValueByKey("visitorCount").toInt();
        if (visitorType == "2")
        {
            if (visitorCount > 1)
            {
                isRet = false;
                break;
            }
        }
        else if (visitorType == "3")
        {
            if (visitorCount < 2)
            {
                isRet = false;
                break;
            }
        }

        QString deviceType = filterInfo.GetValueByKey("deviceType");
        QString device     = GetUIVisitorInfoValueByKey("device");
        if (deviceType == "2")
        {
            if (device != "pc")
            {
                isRet = false;
                break;
            }
        }
        else if (deviceType == "3")
        {
            if (device != "phone")
            {
                isRet = false;
                break;
            }
        }

        QString area = filterInfo.GetValueByKey("area");
        if (!area.isEmpty())
        {
            if (!GetUIVisitorInfoValueByKey("visitorLocationCity").contains(area) && !GetUIVisitorInfoValueByKey("visitorLocationProvince").contains(area) && !GetUIVisitorInfoValueByKey("visitorLocationCountry").contains(area))
            {
                isRet = false;
                break;
            }
        }

        QString firstUrl = filterInfo.GetValueByKey("firstUrl");
        if (!firstUrl.isEmpty())
        {
            if (!GetUIVisitorInfoValueByKey("firstActiveUrl").contains(firstUrl))
            {
                isRet = false;
                break;
            }
        }

        QString lastUrl = filterInfo.GetValueByKey("lastUrl");
        if (!lastUrl.isEmpty())
        {
            if (!GetUIVisitorInfoValueByKey("lastActiveUrl").contains(lastUrl))
            {
                isRet = false;
                break;
            }
        }
        QString keyWord = filterInfo.GetValueByKey("keyWord");
        if (!keyWord.isEmpty())
        {
            if (!GetUIVisitorInfoValueByKey("keyWord").contains(keyWord))
            {
                isRet = false;
                break;
            }
        }

        QString refUrl = filterInfo.GetValueByKey("refUrl");
        if (!refUrl.isEmpty())
        {
            if (!GetUIVisitorInfoValueByKey("urlRefer").contains(refUrl))
            {
                isRet = false;
                break;
            }
        }

        QString ip = filterInfo.GetValueByKey("ip");
        if (!ip.isEmpty())
        {
            if (!GetUIVisitorInfoValueByKey("ip").contains(ip))
            {
                isRet = false;
                break;
            }
        }
    } while (false);
    return isRet;
}

bool MMonitorVisitorData::IsContainByKey(QString strKey, QString val)
{
    bool    ret      = false;
    QString firstUrl = GetUIVisitorInfoValueByKey(strKey);
    if (firstUrl.contains(val))
    {
        ret = true;
    }
    return ret;
}
