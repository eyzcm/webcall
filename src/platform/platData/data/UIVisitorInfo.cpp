#include "UIVisitorInfo.h"
#include "utils/utilshelper.h"

UIVisitorInfo::UIVisitorInfo(void)
{
    m_visitorStatus    = E_VSTATUS_DEFAULT;
    m_isHide           = false;
    m_cardInfo         = nullptr;
    m_extend_keyWord   = "";
    m_strSearchingType = "";
    m_strAwake         = "";
    m_strAwakeInfo     = "";
    m_strNickName      = "";
}

UIVisitorInfo::UIVisitorInfo(UIVisitorInfo* pinfo)
{
    if (pinfo == NULL)
    {
        m_visitorStatus    = E_VSTATUS_DEFAULT;
        m_isHide           = false;
        m_cardInfo         = nullptr;
        m_extend_keyWord   = "";
        m_strSearchingType = "";
        m_strAwake         = "";
        m_strAwakeInfo     = "";
        m_strNickName      = "";
        return;
    }

    this->AcceptCount(pinfo->AcceptCount());
    this->ChatUrl(pinfo->ChatUrl());
    this->CreateTime(pinfo->CreateTime());
    this->CustomerId(pinfo->CustomerId());
    this->CustomerName(pinfo->CustomerName());
    this->DenyCount(pinfo->DenyCount());
    this->Device(pinfo->Device());
    this->FirstActiveUrl(pinfo->FirstActiveUrl());
    this->Ie(pinfo->Ie());
    this->InviteCount(pinfo->InviteCount());
    this->Ip(pinfo->Ip());
    this->KeyWord(pinfo->KeyWord());
    this->LastActiveUrl(pinfo->LastActiveUrl());
    this->LastActiveUrlTitle(pinfo->LastActiveUrlTitle());
    this->FirstActiveUrlTitile(pinfo->FirstActiveUrlTitile());
    this->LocalOption(pinfo->LocalOption());
    this->Os(pinfo->Os());
    this->UserAgent(pinfo->UserAgent());
    this->Chroma(pinfo->Chroma());
    this->Lang(pinfo->Lang());
    this->DeviceModel(pinfo->DeviceModel());
    this->Resolution(pinfo->Resolution());
    this->SearchEngineId(pinfo->SearchEngineId());
    this->SearchEngineName(pinfo->SearchEngineName());
    this->HeadImgUrl(pinfo->HeadImgUrl());
    this->Gender(pinfo->Gender());
    this->SiteId(pinfo->SiteId());
    this->StaticId(pinfo->StaticId());
    this->UrlAlias(pinfo->UrlAlias());
    this->UrlRefer(pinfo->UrlRefer());
    this->UrlTitle(pinfo->UrlTitle());
    this->Vid(pinfo->Vid());
    this->VisitorCount(pinfo->VisitorCount());
    this->VisitorName(pinfo->VisitorName());
    this->VisitorStartTime(pinfo->VisitorStartTime());
    this->VisitorStatus(pinfo->VisitorStatus());
    this->VisitorType(pinfo->VisitorType());
    this->Visitor_location_city(pinfo->Visitor_location_city());
    this->Visitor_location_district(pinfo->Visitor_location_district());
    this->Visitor_location_country(pinfo->Visitor_location_country());
    this->Visitor_location_province(pinfo->Visitor_location_province());
    this->SpreadFlag(pinfo->SpreadFlag());
    this->PromotionId(pinfo->PromotionId());
    this->PromotionName(pinfo->PromotionName());
    this->ViewPageCount(pinfo->ViewPageCount());
    this->IsHide(pinfo->IsHide());
    this->HasCard(pinfo->HasCard());
    this->Extend_KeyWord(pinfo->Extend_KeyWord());

    this->SearchingType(pinfo->SearchingType());
    this->Awake(pinfo->Awake());
    this->AwakeInfo(pinfo->AwakeInfo());
    this->NickName(pinfo->NickName());
    this->PreCustomerId(pinfo->PreCustomerId());

    if (pinfo->CardInfo())
    {
        this->m_cardInfo = new UICardInfo(*pinfo->CardInfo());
    }
    else
    {
        this->m_cardInfo = nullptr;
    }
}

UIVisitorInfo::UIVisitorInfo(const UIVisitorInfo& info)
{
    this->AcceptCount(info.AcceptCount());
    this->ChatUrl(info.ChatUrl());
    this->CreateTime(info.CreateTime());
    this->CustomerId(info.CustomerId());
    this->CustomerName(info.CustomerName());
    this->DenyCount(info.DenyCount());
    this->Device(info.Device());
    this->FirstActiveUrl(info.FirstActiveUrl());
    this->Ie(info.Ie());
    this->InviteCount(info.InviteCount());
    this->Ip(info.Ip());
    this->KeyWord(info.KeyWord());
    this->LastActiveUrl(info.LastActiveUrl());
    this->LastActiveUrlTitle(info.LastActiveUrlTitle());
    this->FirstActiveUrlTitile(info.FirstActiveUrlTitile());
    this->LocalOption(info.LocalOption());
    this->Os(info.Os());
    this->UserAgent(info.UserAgent());
    this->DeviceModel(info.DeviceModel());
    this->Chroma(info.Chroma());
    this->Lang(info.Lang());
    this->Resolution(info.Resolution());
    this->SearchEngineId(info.SearchEngineId());
    this->SearchEngineName(info.SearchEngineName());
    this->HeadImgUrl(info.HeadImgUrl());
    this->Gender(info.Gender());
    this->SiteId(info.SiteId());
    this->StaticId(info.StaticId());
    this->UrlAlias(info.UrlAlias());
    this->UrlRefer(info.UrlRefer());
    this->UrlTitle(info.UrlTitle());
    this->Vid(info.Vid());
    this->VisitorCount(info.VisitorCount());
    this->VisitorName(info.VisitorName());
    this->VisitorStartTime(info.VisitorStartTime());
    this->VisitorStatus(info.VisitorStatus());
    this->VisitorType(info.VisitorType());
    this->Visitor_location_city(info.Visitor_location_city());
    this->Visitor_location_district(info.Visitor_location_district());
    this->Visitor_location_country(info.Visitor_location_country());
    this->Visitor_location_province(info.Visitor_location_province());
    this->SpreadFlag(info.SpreadFlag());
    this->PromotionId(info.PromotionId());
    this->PromotionName(info.PromotionName());
    this->ViewPageCount(info.ViewPageCount());
    this->IsHide(info.IsHide());
    this->HasCard(info.HasCard());
    this->Extend_KeyWord(info.Extend_KeyWord());

    this->SearchingType(info.SearchingType());
    this->Awake(info.Awake());
    this->AwakeInfo(info.AwakeInfo());
    this->NickName(info.NickName());
    this->PreCustomerId(info.PreCustomerId());

    if (info.CardInfo())
    {
        this->m_cardInfo = new UICardInfo(*info.CardInfo());
    }
    else
    {
        this->m_cardInfo = nullptr;
    }
}

UIVisitorInfo::~UIVisitorInfo(void)
{
    if (m_cardInfo)
        delete m_cardInfo;
}

QString UIVisitorInfo::GetToolTip() const
{
    // IP地址  ip地址显示的地区 来访次数 当前页面  收索引擎  关键字 本次进入网站时间。（顺序按照文字顺序）
    // IP:
    // IP定位：
    //来访次数：
    //当前页面：
    //搜索引擎：
    //关键字：
    //本次进入网站时间：

    QString format = "IP:%1\r\nIP定位：%2\r\n来访次数：%3\r\n当前页面：%4\r\n搜索引擎：%5\r\n关键字：%6\r\n本次进入网站时间：%7\r\n";

    QString activeTime = UtilsHelper::GetTimeByMSecs(m_visitorStartTime);

    QString ret = format.arg(m_ip).arg(m_localOption).arg(m_visitorCount).arg(m_lastActiveUrl).arg(m_searchEngineId).arg(m_keyWord).arg(activeTime);
    return ret;
}

void UIVisitorInfo::VisitorStatus(EVISITORSTATUS val)
{
    m_visitorStatus = val;
}

EVISITORSTATUS UIVisitorInfo::VisitorStatus() const
{
    return m_visitorStatus;
}

void UIVisitorInfo::Vid(QString val)
{
    m_vid = val;
}

QString UIVisitorInfo::Vid() const
{
    return m_vid;
}

void UIVisitorInfo::StaticId(QString val)
{
    m_staticId = val;
}

QString UIVisitorInfo::StaticId() const
{
    return m_staticId;
}

QString UIVisitorInfo::DisplayName() const
{

    QString ret = m_visitorName;
    if( m_visitorName.isEmpty() ) ret = m_strNickName;
    if (ret.isEmpty())
    {
        ret              = m_vid;
        QString position = m_visitor_location_city;
        if (position.isEmpty())
        {
            position = m_visitor_location_province;
        }
        else
        {
            position += m_visitor_location_district;
        }

        if (position.isEmpty())
        {
            position = m_visitor_location_country;
        }
        ret = UtilsHelper::GetDefaultName(position, ret);
    }
    return ret;
}

void UIVisitorInfo::VisitorStartTime(QString val)
{
    m_visitorStartTime = val;
}

QString UIVisitorInfo::VisitorStartTime() const
{
    return m_visitorStartTime;
}

void UIVisitorInfo::LocalOption(QString val)
{
    m_localOption = val;
}

QString UIVisitorInfo::LocalOption() const
{
    return m_localOption;
}

void UIVisitorInfo::ChatUrl(QString val)
{
    m_chatUrl = val;
}

QString UIVisitorInfo::ChatUrl() const
{
    return m_chatUrl;
}

void UIVisitorInfo::UrlAlias(QString val)
{
    m_urlAlias = val;
}

QString UIVisitorInfo::UrlAlias() const
{
    return m_urlAlias;
}

void UIVisitorInfo::UrlTitle(QString val)
{
    m_urlTitle = val;
}

QString UIVisitorInfo::UrlTitle() const
{
    return m_urlTitle;
}

void UIVisitorInfo::UrlRefer(QString val)
{
    m_urlRefer = UtilsHelper::URLtostr(val);
}

QString UIVisitorInfo::UrlRefer() const
{
    return m_urlRefer;
}

void UIVisitorInfo::SearchEngineId(QString val)
{
    m_searchEngineId = val;
}

QString UIVisitorInfo::SearchEngineId() const
{
    return m_searchEngineId;
}

void UIVisitorInfo::SearchEngineName(QString val)
{
    m_searchEngineName = val;
}

QString UIVisitorInfo::SearchEngineName() const
{
    return m_searchEngineName;
}

void UIVisitorInfo::KeyWord(QString val)
{
    m_keyWord = val;
}

QString UIVisitorInfo::KeyWord() const
{
    return m_keyWord;
}

void UIVisitorInfo::CustomerId(QString val)
{
    m_customerId = val;
}

QString UIVisitorInfo::CustomerId() const
{
    return m_customerId;
}

void UIVisitorInfo::CustomerName(QString val)
{
    m_customerName = val;
}

QString UIVisitorInfo::CustomerName() const
{
    return m_customerName;
}

void UIVisitorInfo::VisitorCount(QString val)
{
    m_visitorCount = val;
}

QString UIVisitorInfo::VisitorCount() const
{
    return m_visitorCount;
}

void UIVisitorInfo::InviteCount(QString val)
{
    m_inviteCount = val;
}

QString UIVisitorInfo::InviteCount() const
{
    return m_inviteCount;
}

void UIVisitorInfo::AcceptCount(QString val)
{
    m_acceptCount = val;
}

QString UIVisitorInfo::AcceptCount() const
{
    return m_acceptCount;
}

void UIVisitorInfo::DenyCount(QString val)
{
    m_denyCount = val;
}

QString UIVisitorInfo::DenyCount() const
{
    return m_denyCount;
}

void UIVisitorInfo::SiteId(QString val)
{
    m_siteId = val;
}

QString UIVisitorInfo::SiteId() const
{
    return m_siteId;
}

void UIVisitorInfo::VisitorType(QString val)
{
    m_visitorType = val;
}

QString UIVisitorInfo::VisitorType() const
{
    return m_visitorType;
}

void UIVisitorInfo::VisitorName(QString val)
{
    m_visitorName = val;
}

QString UIVisitorInfo::VisitorName() const
{
    if( m_visitorName.isEmpty() )return m_strNickName;
    return m_visitorName;
}

void UIVisitorInfo::Visitor_location_province(QString val)
{
    m_visitor_location_province = val;
}

QString UIVisitorInfo::Visitor_location_province() const
{
    return m_visitor_location_province;
}

void UIVisitorInfo::Visitor_location_city(QString val)
{
    m_visitor_location_city = val;
}

QString UIVisitorInfo::Visitor_location_city() const
{
    return m_visitor_location_city;
}

void UIVisitorInfo::Visitor_location_district(QString val)
{
    m_visitor_location_district = val;
}

QString UIVisitorInfo::Visitor_location_district() const
{
    return m_visitor_location_district;
}

void UIVisitorInfo::Visitor_location_country(QString val)
{
    m_visitor_location_country = val;
}

QString UIVisitorInfo::Visitor_location_country() const
{
    return m_visitor_location_country;
}

void UIVisitorInfo::LastActiveUrl(QString val)
{
    m_lastActiveUrl = val;
}

QString UIVisitorInfo::LastActiveUrl() const
{
    return m_lastActiveUrl;
}

void UIVisitorInfo::FirstActiveUrl(QString val)
{
    m_firstActiveUrl = val;
}

QString UIVisitorInfo::FirstActiveUrl() const
{
    return m_firstActiveUrl;
}

void UIVisitorInfo::Ip(QString val)
{
    m_ip = val;
}

QString UIVisitorInfo::Ip() const
{
    return m_ip;
}

void UIVisitorInfo::Ie(QString val)
{
    m_ie = val;
}

QString UIVisitorInfo::Ie() const
{
    return m_ie;
}

void UIVisitorInfo::CreateTime(QString val)
{
    m_createTime = val;
}

QString UIVisitorInfo::CreateTime() const
{
    return m_createTime;
}

void UIVisitorInfo::LastActiveUrlTitle(QString val)
{
    m_lastActiveUrlTitle = val;
}

QString UIVisitorInfo::LastActiveUrlTitle() const
{
    return m_lastActiveUrlTitle;
}

void UIVisitorInfo::Resolution(QString val)
{
    m_resolution = val;
}

QString UIVisitorInfo::Resolution() const
{
    return m_resolution;
}

void UIVisitorInfo::Os(QString val)
{
    m_os = val;
}

QString UIVisitorInfo::Os() const
{
    return m_os;
}

void UIVisitorInfo::Device(QString val)
{
    m_device = val;
}

QString UIVisitorInfo::Device() const
{
    return m_device;
}

void UIVisitorInfo::Lang(QString val)
{
    m_lang = val;
}

QString UIVisitorInfo::Lang() const
{
    return m_lang;
}

void UIVisitorInfo::Chroma(QString val)
{
    m_chroma = val;
}

QString UIVisitorInfo::Chroma() const
{
    return m_chroma;
}

void UIVisitorInfo::UserAgent(QString val)
{
    m_userAgent = val;
}

QString UIVisitorInfo::UserAgent() const
{
    return m_userAgent;
}

QString UIVisitorInfo::FirstActiveUrlTitile() const
{
    return m_firstActiveUrlTitile;
}

void UIVisitorInfo::FirstActiveUrlTitile(QString val)
{
    m_firstActiveUrlTitile = val;
}

QString UIVisitorInfo::Extend_KeyWord() const
{
    return m_extend_keyWord;
}

void UIVisitorInfo::Extend_KeyWord(QString val)
{
    m_extend_keyWord = val;
}

QString UIVisitorInfo::SearchingType() const
{
    return m_strSearchingType;
}
void UIVisitorInfo::SearchingType(QString val)
{
    m_strSearchingType = val;
}

QString UIVisitorInfo::Awake() const
{
    return m_strAwake;
}
void UIVisitorInfo::Awake(QString val)
{
    m_strAwake = val;
}

QString UIVisitorInfo::AwakeInfo() const
{
    return m_strAwakeInfo;
}
void UIVisitorInfo::AwakeInfo(QString val)
{
    m_strAwakeInfo = val;
}

QString UIVisitorInfo::NickName() const
{
    return m_strNickName;
}
void UIVisitorInfo::NickName(QString val)
{
    m_strNickName = val;
}

QString UIVisitorInfo::PreCustomerId() const
{
    return m_preCustomerId;
}
void UIVisitorInfo::PreCustomerId(QString val)
{
    m_preCustomerId = val;
}

QString UIVisitorInfo::DeviceModel() const
{
    return m_deviceModel;
}
void UIVisitorInfo::DeviceModel(QString val)
{
    m_deviceModel = val;
}


QString UIVisitorInfo::HeadImgUrl() const
{
    return m_headImgUrl;
}

void UIVisitorInfo::HeadImgUrl(QString val)
{
    m_headImgUrl = val;
}

QString UIVisitorInfo::Gender() const
{
    return m_gender;
}

void UIVisitorInfo::Gender(QString val)
{
    m_gender = val;
}
