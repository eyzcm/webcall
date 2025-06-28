#include "MMonitorChatData.h"
 #include "utils/DefineHelper.h"

MMonitorChatData::MMonitorChatData(void)
{

}

MMonitorChatData::MMonitorChatData(const MMonitorChatData& o)
{
    Vid(o.Vid());
    Userid(o.Userid());
    CustomerId(o.CustomerId());
    CustomerName(o.CustomerName());
    ChatId(o.ChatId());
    Name(o.Name());
    VisitorInfoMap(o.VisitorInfoMap());

    SearchingType(o.SearchingType());
    Awake(o.Awake());
    AwakeInfo(o.AwakeInfo());
    NickName(o.NickName());
    ChatCloseTime(o.ChatCloseTime());

    ExtendInfo(o.ExtendInfo());
}

MMonitorChatData::~MMonitorChatData(void)
{
}

void MMonitorChatData::Userid(QString val)
{
    m_userid = val;
}

QString MMonitorChatData::Userid() const
{
    return m_userid;
}

void MMonitorChatData::Vid(QString val)
{
    m_vid = val;
}

QString MMonitorChatData::Vid() const
{
    return m_vid;
}

void MMonitorChatData::Name(QString val)
{
    m_name = val;
}

QString MMonitorChatData::Name() const
{
    return m_name;
}

void MMonitorChatData::InitChatInfo(const UIChatInfo& info,QString name,QString departmentId, QString departmentName)
{
    if (!info.ChatId().isEmpty())
    {
        UIChatUserInfo* visitorInfo = nullptr;
        if (info.GetVisitorInfo())
        {
            visitorInfo = info.GetVisitorInfo();
        }
        if (info.GetVisitorInfo())
        {
            Userid(info.GetVisitorInfo()->StaticId());
            Vid(info.GetVisitorInfo()->UserId());
        }

        ChatId(info.ChatId());
        Name(name);
        CustomerId(info.CustomerID());
        CustomerName(info.CustomerName());

        SearchingType(info.SearchingType());
        Awake(info.Awake());
        AwakeInfo(info.AwakeInfo());
        NickName(info.NickName());
        ChatCloseTime(info.ChatCloseTime());

        if( !departmentId.isEmpty()  ) m_visitorInfoMap.insert("departmentId",departmentId);
        if( !departmentName.isEmpty()  ) m_visitorInfoMap.insert("departmentName", departmentName);

        m_visitorInfoMap.insert("name", Name());
        m_visitorInfoMap.insert("customerName", CustomerName());
        m_visitorInfoMap.insert("userId", Userid());
        m_visitorInfoMap.insert("vid", Vid());
        m_visitorInfoMap.insert("chatId", ChatId());
        m_visitorInfoMap.insert("customerId", CustomerId());
        m_visitorInfoMap.insert("createTime", info.CreateTime());
        m_visitorInfoMap.insert("chatUrl", info.ChatUrl());
        m_visitorInfoMap.insert("device", info.Chat_type());
        m_visitorInfoMap.insert("visitorLocationCity", info.Visitor_location_city());
        m_visitorInfoMap.insert("visitorLocationProvince", info.Visitor_location_province());
        m_visitorInfoMap.insert("visitorLocationCountry", info.Visitor_location_country());
        m_visitorInfoMap.insert("visitorLocationDistrict", info.Visitor_location_district());
        m_visitorInfoMap.insert("visitorMsgCount", info.VisitorMsgCount());
        m_visitorInfoMap.insert("userMsgCount", info.UserMsgCount());
        m_visitorInfoMap.insert("visitor_ip", info.Visitor_ip());
        m_visitorInfoMap.insert("ip", info.Visitor_ip());

        if (visitorInfo && visitorInfo->OtherInfos())
        {
            QString siteId = visitorInfo->OtherInfos()->GetValueByKey("siteId");
 //           m_visitorInfoMap.insert("siteName", gUIOtherServiceManager->GetSiteNameById(siteId));
            m_visitorInfoMap.insert("siteId", siteId);
            m_visitorInfoMap.insert("searchEngineId", visitorInfo->OtherInfos()->GetValueByKey("searchEngineId"));
            m_visitorInfoMap.insert("groupId", visitorInfo->OtherInfos()->GetValueByKey("groupId"));
            m_visitorInfoMap.insert("searchHost", visitorInfo->OtherInfos()->GetValueByKey("searchHost"));
            m_visitorInfoMap.insert("searching", visitorInfo->OtherInfos()->GetValueByKey("searching"));
            m_visitorInfoMap.insert("promotionId", visitorInfo->OtherInfos()->GetValueByKey("promotionId"));
            m_visitorInfoMap.insert("promotionName", visitorInfo->OtherInfos()->GetValueByKey("promotionName"));
            m_visitorInfoMap.insert("keyWord", visitorInfo->OtherInfos()->GetValueByKey("keyWord"));
            m_visitorInfoMap.insert("firstUrl", visitorInfo->OtherInfos()->GetValueByKey("firstUrl"));
            m_visitorInfoMap.insert("urlRefer", visitorInfo->OtherInfos()->GetValueByKey("refer_page_url"));
            m_visitorInfoMap.insert("clickText", visitorInfo->OtherInfos()->GetValueByKey("clickText"));
            m_visitorInfoMap.insert("xst", visitorInfo->OtherInfos()->GetValueByKey("xst"));
        }
    }
}

QString MMonitorChatData::GetUIChatInfoValueByKey(QString strKey) const
{
    QString ret = "";
    if (m_visitorInfoMap.find(strKey) != m_visitorInfoMap.end())
    {
        ret = m_visitorInfoMap[strKey];
    }
    return ret;
}

void MMonitorChatData::SetUIChatInfoValueByKey(QString strKey, QString strVal)
{
    if (m_visitorInfoMap.find(strKey) != m_visitorInfoMap.end())
    {
        m_visitorInfoMap.insert(strKey, strVal);
    }
}

QString MMonitorChatData::NickName() const
{
    return m_strNickName;
}
void MMonitorChatData::NickName(QString val)
{
    m_strNickName = val;
}

QDateTime MMonitorChatData::ChatCloseTime() const
{
    return m_chatCloseTime;
}

void MMonitorChatData::ChatCloseTime(QDateTime val)
{
    m_chatCloseTime = val;
}

void MMonitorChatData::VisitorInfoMap(QMap<QString, QString> val)
{
    m_visitorInfoMap = val;
}

QMap<QString, QString> MMonitorChatData::VisitorInfoMap() const
{
    return m_visitorInfoMap;
}

void MMonitorChatData::CustomerId(QString val)
{
    m_customerId = val;
}

QString MMonitorChatData::CustomerId() const
{
    return m_customerId;
}

void MMonitorChatData::ChatId(QString val)
{
    m_chatId = val;
}

QString MMonitorChatData::ChatId() const
{
    return m_chatId;
}

QString MMonitorChatData::CustomerName() const
{
    return m_customerName;
}

void MMonitorChatData::CustomerName(QString val)
{
    m_customerName = val;
}

bool MMonitorChatData::IsFilter(QString searchKey, CommInfo filterInfo)
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

        QString chatUrl = filterInfo.GetValueByKey("chatUrl");
        if (!chatUrl.isEmpty())
        {
            if (!GetUIChatInfoValueByKey("chatUrl").contains(chatUrl))
            {
                isRet = false;
                break;
            }
        }

        QString keyWord = filterInfo.GetValueByKey("keyWord");
        if (!keyWord.isEmpty())
        {
            if (!GetUIChatInfoValueByKey("keyWord").contains(keyWord))
            {
                isRet = false;
                break;
            }
        }

        QString refUrl = filterInfo.GetValueByKey("refUrl");
        if (!refUrl.isEmpty())
        {
            if (!GetUIChatInfoValueByKey("urlRefer").contains(refUrl))
            {
                isRet = false;
                break;
            }
        }

        QString ip = filterInfo.GetValueByKey("ip");
        if (!ip.isEmpty())
        {
            if (!GetUIChatInfoValueByKey("visitor_ip").contains(ip))
            {
                isRet = false;
                break;
            }
        }

        QString department = filterInfo.GetValueByKey("department");
        if (!department.isEmpty())
        {
            if (!GetUIChatInfoValueByKey("departmentName").contains(department))
            {
                isRet = false;
                break;
            }
        }

        QString customer = filterInfo.GetValueByKey("customer");
        if (!customer.isEmpty())
        {
            if (!GetUIChatInfoValueByKey("customerName").contains(customer))
            {
                isRet = false;
                break;
            }
        }

    } while (false);
    return isRet;
}

void MMonitorChatData::CloneData(const MMonitorChatData& o)
{
    Vid(o.Vid());
    Userid(o.Userid());
    CustomerId(o.CustomerId());
    CustomerName(o.CustomerName());
    ChatId(o.ChatId());
    Name(o.Name());
    VisitorInfoMap(o.VisitorInfoMap());
    ExtendInfo(o.ExtendInfo());

    SearchingType(o.SearchingType());
    Awake(o.Awake());
    AwakeInfo(o.AwakeInfo());
    NickName(o.NickName());
    ChatCloseTime(o.ChatCloseTime());
}

QString MMonitorChatData::SearchingType() const
{
    return m_strSearchingType;
}
void MMonitorChatData::SearchingType(QString val)
{
    m_strSearchingType = val;
}

QString MMonitorChatData::Awake() const
{
    return m_strAwake;
}
void MMonitorChatData::Awake(QString val)
{
    m_strAwake = val;
}

QString MMonitorChatData::AwakeInfo() const
{
    return m_strAwakeInfo;
}
void MMonitorChatData::AwakeInfo(QString val)
{
    m_strAwakeInfo = val;
}
