#include "UIChatInfo.h"
#include "utils/EL_Util_Time.h"

UIChatInfo::UIChatInfo(QObject *parent) : QObject(parent)
{
    m_isFlash    = true;
    m_isNewChat  = false;
    m_inviteMode = 2;
    m_aiChatFlag = 0;

    m_strSummaryTag     = "";
    m_strChatAwaken     = "";
    m_strSearchingType  = "";
    m_strAwake          = "";
    m_strAwakeInfo      = "";
    m_strNickName       = "";
    m_effectiveAllocate = "0";
    m_isAckLeave        = false;
    m_takeoverFlag      = 0;
    m_ssid              = "";

    m_type          = E_MSGTYPE_DEFAULT;
    m_chatCloseTime = QDateTime::currentDateTime();

    BatchId(0);
}

UIChatInfo::UIChatInfo(const UIChatInfo& info)
{
    this->PreCustomerId(info.PreCustomerId());
    this->TokeOverID(info.TokeOverID());
    this->ChatId(info.ChatId());
    this->ChatUrl(info.ChatUrl());
    this->MsgType(info.MsgType());
    this->Chat_type(info.Chat_type());
    this->CreateTime(info.CreateTime());
    this->CustomerID(info.CustomerID());
    this->CustomerName(info.CustomerName());
    this->SiteId(info.SiteId());
    this->TempChat(info.TempChat());
    this->TransferUserId(info.TransferUserId());
    this->UserMsgCount(info.UserMsgCount());
    this->VisitorMsgCount(info.VisitorMsgCount());
    this->Visitor_ip(info.Visitor_ip());
    this->Visitor_ipv6(info.Visitor_ipv6());
    this->Visitor_location_city(info.Visitor_location_city());
    this->Visitor_location_district(info.Visitor_location_district());
    this->Visitor_location_country(info.Visitor_location_country());
    this->Visitor_location_province(info.Visitor_location_province());
    this->Flash(info.Flash());
    this->NewChat(info.NewChat());
    this->InviteMode(info.InviteMode());
    this->AiChatFlag(info.AiChatFlag());
    this->BatchId(info.BatchId());

    this->SummaryTag(info.SummaryTag());
    this->ChatAwaken(info.ChatAwaken());
    this->SearchingType(info.SearchingType());
    this->Awake(info.Awake());
    this->AwakeInfo(info.AwakeInfo());
    this->NickName(info.NickName());
    this->ChatCloseTime(info.ChatCloseTime());
    this->IsAckLeave(info.IsAckLeave());
    this->TakeoverFlag(info.TakeoverFlag());
    this->Ssid(info.Ssid());
    this->SensitiveWords(info.SensitiveWords());

    this->EffectiveAllocate(info.EffectiveAllocate());

    Q_FOREACH (UIChatUserInfo* obj, info.ChatUserList())
    {
        if (obj)
        {
            UIChatUserInfo* tempObj = new UIChatUserInfo(*obj);
            m_uiChatUserList.push_back(tempObj);
        }
    }
}

UIChatInfo::UIChatInfo(UIChatInfo* pinfo)
{
    if (pinfo != NULL)
    {
        this->TokeOverID(pinfo->TokeOverID());
        this->ChatId(pinfo->ChatId());
        this->ChatUrl(pinfo->ChatUrl());
        this->Chat_type(pinfo->Chat_type());
        this->CreateTime(pinfo->CreateTime());
        this->CustomerID(pinfo->CustomerID());
        this->CustomerName(pinfo->CustomerName());
        this->SiteId(pinfo->SiteId());
        this->TempChat(pinfo->TempChat());
        this->TransferUserId(pinfo->TransferUserId());
        this->UserMsgCount(pinfo->UserMsgCount());
        this->VisitorMsgCount(pinfo->VisitorMsgCount());
        this->Visitor_ip(pinfo->Visitor_ip());
        this->Visitor_ipv6(pinfo->Visitor_ipv6());
        this->Visitor_location_city(pinfo->Visitor_location_city());
        this->Visitor_location_district(pinfo->Visitor_location_district());
        this->Visitor_location_country(pinfo->Visitor_location_country());
        this->Visitor_location_province(pinfo->Visitor_location_province());
        this->PreCustomerId(pinfo->PreCustomerId());
        this->Flash(pinfo->Flash());
        this->NewChat(pinfo->NewChat());
        this->InviteMode(pinfo->InviteMode());
        this->AiChatFlag(pinfo->AiChatFlag());
        this->BatchId(pinfo->BatchId());
        this->MsgType(pinfo->MsgType());
        this->SummaryTag(pinfo->SummaryTag());
        this->ChatAwaken(pinfo->ChatAwaken());
        this->SearchingType(pinfo->SearchingType());
        this->Awake(pinfo->Awake());
        this->AwakeInfo(pinfo->AwakeInfo());
        this->NickName(pinfo->NickName());
        this->ChatCloseTime(pinfo->ChatCloseTime());
        this->EffectiveAllocate(pinfo->EffectiveAllocate());
        this->IsAckLeave(pinfo->IsAckLeave());
        this->TakeoverFlag(pinfo->TakeoverFlag());
        this->Ssid(pinfo->Ssid());
        this->SensitiveWords(pinfo->SensitiveWords());

        Q_FOREACH (UIChatUserInfo* obj, pinfo->ChatUserList())
        {
            if (obj)
            {
                UIChatUserInfo* tempObj = new UIChatUserInfo(*obj);
                m_uiChatUserList.push_back(tempObj);
            }
        }
    }
    else
    {
        m_isFlash   = true;
        m_isNewChat = false;
        //邀请模式
        m_inviteMode        = 2;
        m_aiChatFlag        = 0;
        m_tokeoverID        = "";
        m_strSummaryTag     = "";
        m_strChatAwaken     = "";
        m_type              = E_MSGTYPE_DEFAULT;
        m_strSearchingType  = "";
        m_strAwake          = "";
        m_strAwakeInfo      = "";
        m_strNickName       = "";
        m_effectiveAllocate = "0";
        m_chatCloseTime     = QDateTime::currentDateTime();
        m_isAckLeave        = false;
        m_takeoverFlag      = 0;

        BatchId(0);
    }
}

UIChatInfo::~UIChatInfo(void)
{
    qDeleteAll(m_uiChatUserList);
    m_uiChatUserList.clear();
}

QString UIChatInfo::GetKeyword()
{
    QString keyword = "";
    Q_FOREACH (UIChatUserInfo* obj, m_uiChatUserList)
    {
        if (obj && obj->UserType() == E_VISITOR)
        {
            keyword = obj->OtherInfos()->GetValueByKey("keyWord");

            break;
        }
    }
    return keyword;
}

void UIChatInfo::ChatId(QString val)
{
    m_chatId = val;
}

QString UIChatInfo::ChatId() const
{
    return m_chatId;
}

void UIChatInfo::CustomerID(QString val)
{
    m_customerID = val;
}

QString UIChatInfo::CustomerID() const
{
    return m_customerID;
}

void UIChatInfo::CustomerName(QString val)
{
    m_customerName = val;
}

QString UIChatInfo::CustomerName() const
{
    return m_customerName;
}

void UIChatInfo::CreateTime(QString val)
{
    m_createTime = val;
}

QString UIChatInfo::CreateTime() const
{
    return m_createTime;
}

void UIChatInfo::UserMsgCount(QString val)
{
    m_userMsgCount = val;
}

QString UIChatInfo::UserMsgCount() const
{
    return m_userMsgCount;
}

void UIChatInfo::VisitorMsgCount(QString val)
{
    m_visitorMsgCount = val;
}

QString UIChatInfo::VisitorMsgCount() const
{
    return m_visitorMsgCount;
}

void UIChatInfo::ChatUrl(QString val)
{
    m_chatUrl = val;
}

QString UIChatInfo::ChatUrl() const
{
    return m_chatUrl;
}

void UIChatInfo::Visitor_location_province(QString val)
{
    m_visitor_location_province = val;
}

QString UIChatInfo::Visitor_location_province() const
{
    return m_visitor_location_province;
}

void UIChatInfo::Visitor_location_city(QString val)
{
    m_visitor_location_city = val;
}

QString UIChatInfo::Visitor_location_city() const
{
    return m_visitor_location_city;
}

void UIChatInfo::Visitor_location_district(QString val)
{
    m_visitor_location_district = val;
}

QString UIChatInfo::Visitor_location_district() const
{
    return m_visitor_location_district;
}

void UIChatInfo::Visitor_location_country(QString val)
{
    m_visitor_location_country = val;
}

QString UIChatInfo::Visitor_location_country() const
{
    return m_visitor_location_country;
}

void UIChatInfo::Visitor_ip(QString val)
{
    m_visitor_ip = val;
}

QString UIChatInfo::Visitor_ip() const
{
    return m_visitor_ip;
}

QString UIChatInfo::Visitor_ipv6() const
{
    return m_visitor_ipv6;
}

void UIChatInfo::Visitor_ipv6(QString val)
{
    m_visitor_ipv6 = val;
}

void UIChatInfo::Chat_type(QString val)
{
    m_chat_type = val;
}

QString UIChatInfo::Chat_type() const
{
    return m_chat_type;
}

void UIChatInfo::SiteId(QString val)
{
    m_siteId = val;
}

QString UIChatInfo::SiteId() const
{
    return m_siteId;
}

void UIChatInfo::TempChat(bool val)
{
    m_isTempChat = val;
}

bool UIChatInfo::TempChat() const
{
    return m_isTempChat;
}

void UIChatInfo::TransferUserId(QString val)
{
    m_transferUserId = val;
}

QString UIChatInfo::TransferUserId() const
{
    return m_transferUserId;
}

void UIChatInfo::AddChatUser(UIChatUserInfo* userinfo)
{

    m_uiChatUserList.push_back(userinfo);
}

bool UIChatInfo::HasVisitorByVid(const QString& vid)
{

    bool ret = false;
    Q_FOREACH (UIChatUserInfo* obj, m_uiChatUserList)
    {
        if (obj && obj->UserType() == E_VISITOR && obj->UserId() == vid)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

bool UIChatInfo::HasVisitorByUserId(const QString& userid)
{
    bool ret = false;
    Q_FOREACH (UIChatUserInfo* obj, m_uiChatUserList)
    {
        if (obj && obj->UserType() == E_VISITOR && obj->StaticId() == userid)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

UIChatUserInfo* UIChatInfo::GetChatUserInfoByVid(const QString& vid) const
{
    UIChatUserInfo* ret = nullptr;
    Q_FOREACH (UIChatUserInfo* obj, m_uiChatUserList)
    {
        if (obj && obj->UserId() == vid)
        {
            ret = obj;
            break;
        }
    }
    return ret;
}

UIChatUserInfo* UIChatInfo::GetChatUserInfoByUserId(const QString& userid) const
{
    UIChatUserInfo* ret = nullptr;
    Q_FOREACH (UIChatUserInfo* obj, m_uiChatUserList)
    {
        if (obj && obj->StaticId() == userid)
        {
            ret = obj;
            break;
        }
    }
    return ret;
}

UIChatUserInfo* UIChatInfo::GetVisitorInfo() const
{
    UIChatUserInfo* ret = nullptr;
    Q_FOREACH (UIChatUserInfo* obj, m_uiChatUserList)
    {
        if (obj && obj->UserType() == E_VISITOR)
        {
            ret = obj;
            break;
        }
    }
    return ret;
}

UIChatUserInfo* UIChatInfo::GetCustomerInfo() const
{
    return GetChatUserInfoByVid(m_customerID);
}

QString UIChatInfo::GetDisplayName() const
{
    QString         ret         = ChatId();
    UIChatUserInfo* visitorInfo = GetVisitorInfo();
    if (visitorInfo)
    {
        ret = visitorInfo->GetDisplayName();
    }
    return ret;
}

bool UIChatInfo::IsWatch(QString userid) const
{
    bool ret = false;
    Q_FOREACH (UIChatUserInfo* info, m_uiChatUserList)
    {
        if (info)
        {
            if (info->IsWatcher() && info->UserId() == userid && info->UserType() == E_CUSTOMER)
            {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

QString UIChatInfo::PreCustomerId() const
{
    return m_preCustomerId;
}

void UIChatInfo::PreCustomerId(QString val)
{
    m_preCustomerId = val;
}

QString UIChatInfo::SummaryTag() const
{
    return m_strSummaryTag;
}

void UIChatInfo::SummaryTag(QString val)
{
    m_strSummaryTag = val;
}

QString UIChatInfo::ChatAwaken() const
{
    return m_strChatAwaken;
}

void UIChatInfo::ChatAwaken(QString val)
{
    m_strChatAwaken = val;
}

QString UIChatInfo::SearchingType() const
{
    return m_strSearchingType;
}
void UIChatInfo::SearchingType(QString val)
{
    m_strSearchingType = val;
}

QString UIChatInfo::Awake() const
{
    return m_strAwake;
}
void UIChatInfo::Awake(QString val)
{
    m_strAwake = val;
}

QString UIChatInfo::AwakeInfo() const
{
    return m_strAwakeInfo;
}
void UIChatInfo::AwakeInfo(QString val)
{
    m_strAwakeInfo = val;
}

QString UIChatInfo::NickName() const
{
    return m_strNickName;
}
void UIChatInfo::NickName(QString val)
{
    m_strNickName = val;
}

QDateTime UIChatInfo::ChatCloseTime() const
{
    return m_chatCloseTime;
}

void UIChatInfo::ChatCloseTime(QDateTime val)
{
    m_chatCloseTime = val;
}

UICardInfo* UIChatInfo::CardInfo()
{
    Q_FOREACH (UIChatUserInfo* userInfo, m_uiChatUserList)
    {
        if (userInfo->UserType() == EUSERTYPE::E_VISITOR)
        {
            return userInfo->CardInfo();
        }
    }
    return nullptr;
}

QString UIChatInfo::EffectiveAllocate() const
{
    return m_effectiveAllocate;
}
void UIChatInfo::EffectiveAllocate(QString val)
{
    m_effectiveAllocate = val;
}
QString UIChatInfo::TokeOverID() const
{
    return m_tokeoverID;
}
void UIChatInfo::TokeOverID(QString val)
{
    m_tokeoverID = val;
}

QMap<QString, QString> UIChatInfo::GetExts() const
{
    return m_extsmap;
}

void UIChatInfo::SetExts(QMap<QString, QString> map)
{
    m_extsmap = map;
}

void UIChatInfo::AddExts(QString key, QString val)
{
    m_extsmap.insert(key, val);
}

QString UIChatInfo::SensitiveWords() const
{
    return m_sensitiveWords;
}
void    UIChatInfo::SensitiveWords(QString val)
{
    m_sensitiveWords = val;
}
