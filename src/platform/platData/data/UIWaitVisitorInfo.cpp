#include "UIWaitVisitorInfo.h"
#include "utils/utilshelper.h"

UIWaitVisitorInfo::UIWaitVisitorInfo(void)
{
    m_cardInfo = nullptr;
}

UIWaitVisitorInfo::UIWaitVisitorInfo(UIWaitVisitorInfo* pinfo)
{
    if (pinfo == NULL)
        return;

    this->ChatUrl(pinfo->ChatUrl());
    this->Chat_type(pinfo->Chat_type());
    this->GroupId(pinfo->GroupId());
    this->JoinQueueTime(pinfo->JoinQueueTime());
    this->Queue_index(pinfo->Queue_index());
    this->SiteId(pinfo->SiteId());
    this->UserId(pinfo->UserId());
    this->Visitor_ip(pinfo->Visitor_ip());
    this->Visitor_ipv6(pinfo->Visitor_ipv6());
    this->Visitor_location_city(pinfo->Visitor_location_city());
    this->Visitor_location_district(pinfo->Visitor_location_district());
    this->Visitor_location_country(pinfo->Visitor_location_country());
    this->Visitor_location_province(pinfo->Visitor_location_province());
    this->PromotionId(pinfo->PromotionId());
    this->PromotionName(pinfo->PromotionName());
    this->ViewVid(pinfo->ViewVid());
    this->FirstUrl(pinfo->FirstUrl());
    this->Refer(pinfo->Refer());
    this->KeyWord(pinfo->KeyWord());
    this->HasCard(pinfo->HasCard());
    this->CardInfo(pinfo->CardInfo());
    this->Ie(pinfo->Ie());
    this->Os(pinfo->Os());
    this->UserAgent(pinfo->UserAgent());
    this->Chroma(pinfo->Chroma());
    this->Lang(pinfo->Lang());
    this->DeviceModel(pinfo->DeviceModel());
    this->Resolution(pinfo->Resolution());
}

UIWaitVisitorInfo::UIWaitVisitorInfo(const UIWaitVisitorInfo& info)
{
    this->ChatUrl(info.ChatUrl());
    this->Chat_type(info.Chat_type());
    this->GroupId(info.GroupId());
    this->JoinQueueTime(info.JoinQueueTime());
    this->Queue_index(info.Queue_index());
    this->SiteId(info.SiteId());
    this->UserId(info.UserId());
    this->Visitor_ip(info.Visitor_ip());
    this->Visitor_ipv6(info.Visitor_ipv6());
    this->Visitor_location_city(info.Visitor_location_city());
    this->Visitor_location_district(info.Visitor_location_district());
    this->Visitor_location_country(info.Visitor_location_country());
    this->Visitor_location_province(info.Visitor_location_province());
    this->PromotionId(info.PromotionId());
    this->PromotionName(info.PromotionName());
    this->ViewVid(info.ViewVid());
    this->FirstUrl(info.FirstUrl());
    this->Refer(info.Refer());
    this->KeyWord(info.KeyWord());
    this->HasCard(info.HasCard());
    this->CardInfo(info.CardInfo());
    this->Os(info.Os());
    this->UserAgent(info.UserAgent());
    this->DeviceModel(info.DeviceModel());
    this->Chroma(info.Chroma());
    this->Lang(info.Lang());
    this->Resolution(info.Resolution());
    this->Ie(info.Ie());
}

UIWaitVisitorInfo::~UIWaitVisitorInfo(void)
{
}

void UIWaitVisitorInfo::UserId(QString val)
{
    userId = val;
}

QString UIWaitVisitorInfo::UserId() const
{
    return userId;
}

void UIWaitVisitorInfo::Visitor_location_province(QString val)
{
    m_visitor_location_province = val;
}

QString UIWaitVisitorInfo::Visitor_location_province() const
{
    return m_visitor_location_province;
}

void UIWaitVisitorInfo::Visitor_location_city(QString val)
{
    m_visitor_location_city = val;
}

QString UIWaitVisitorInfo::Visitor_location_city() const
{
    return m_visitor_location_city;
}

void UIWaitVisitorInfo::Visitor_location_district(QString val)
{
    m_visitor_location_district = val;
}

QString UIWaitVisitorInfo::Visitor_location_district() const
{
    return m_visitor_location_district;
}

void UIWaitVisitorInfo::Visitor_location_country(QString val)
{
    m_visitor_location_country = val;
}

QString UIWaitVisitorInfo::Visitor_location_country() const
{
    return m_visitor_location_country;
}

void UIWaitVisitorInfo::GroupId(QString val)
{
    m_groupId = val;
}

QString UIWaitVisitorInfo::GroupId() const
{
    return m_groupId;
}

void UIWaitVisitorInfo::Visitor_ip(QString val)
{
    m_visitor_ip = val;
}

QString UIWaitVisitorInfo::Visitor_ip() const
{
    return m_visitor_ip;
}

QString UIWaitVisitorInfo::Visitor_ipv6() const
{
    return m_visitor_ipv6;
}

void UIWaitVisitorInfo::Visitor_ipv6(QString val)
{
    m_visitor_ipv6 = val;
}

void UIWaitVisitorInfo::Chat_type(QString val)
{
    m_chat_type = val;
}

QString UIWaitVisitorInfo::Chat_type() const
{
    return m_chat_type;
}

void UIWaitVisitorInfo::JoinQueueTime(QString val)
{
    m_joinQueueTime = val;
}

QString UIWaitVisitorInfo::JoinQueueTime() const
{
    return m_joinQueueTime;
}

void UIWaitVisitorInfo::ChatUrl(QString val)
{
    m_chatUrl = val;
}

QString UIWaitVisitorInfo::ChatUrl() const
{
    return m_chatUrl;
}

void UIWaitVisitorInfo::SiteId(QString val)
{
    m_siteId = val;
}

QString UIWaitVisitorInfo::SiteId() const
{
    return m_siteId;
}

void UIWaitVisitorInfo::Queue_index(QString val)
{
    m_queue_index = val;
}

QString UIWaitVisitorInfo::Queue_index() const
{
    return m_queue_index;
}

void UIWaitVisitorInfo::PromotionId(QString val)
{
    m_promotionId = val;
}

QString UIWaitVisitorInfo::PromotionId() const
{
    return m_promotionId;
}


QString UIWaitVisitorInfo::PromotionName() const
{
    return m_promotionName;
}

void UIWaitVisitorInfo::PromotionName(QString val)
{
    m_promotionName = val;
}

void UIWaitVisitorInfo::ViewVid(QString val)
{
    viewVid = val;
}

QString UIWaitVisitorInfo::ViewVid() const
{
    return viewVid;
}

QString UIWaitVisitorInfo::DisplayName() const
{
    QString ret      = viewVid;
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

    return ret;
}

QString UIWaitVisitorInfo::GetVisitorLocation() const
{
    QString city     = Visitor_location_city();
    QString district = Visitor_location_district();
    QString province = Visitor_location_province();
    QString country  = Visitor_location_country();
    QString ip       = Visitor_ip();
    QString ipv6     = Visitor_ipv6();

    QString format = "%1%2%3%4【%5】【%6】";
    return format.arg(country).arg(province).arg(city).arg(district).arg(ip).arg(ipv6);
}

void UIWaitVisitorInfo::Resolution(QString val)
{
    m_resolution = val;
}

QString UIWaitVisitorInfo::Resolution() const
{
    return m_resolution;
}

void UIWaitVisitorInfo::Os(QString val)
{
    m_os = val;
}

QString UIWaitVisitorInfo::Os() const
{
    return m_os;
}

void UIWaitVisitorInfo::Device(QString val)
{
    m_device = val;
}

QString UIWaitVisitorInfo::Device() const
{
    return m_device;
}

QString UIWaitVisitorInfo::DeviceModel() const
{
    return m_deviceModel;
}
void UIWaitVisitorInfo::DeviceModel(QString val)
{
    m_deviceModel = val;
}

void UIWaitVisitorInfo::Lang(QString val)
{
    m_lang = val;
}

QString UIWaitVisitorInfo::Lang() const
{
    return m_lang;
}

void UIWaitVisitorInfo::Chroma(QString val)
{
    m_chroma = val;
}

QString UIWaitVisitorInfo::Chroma() const
{
    return m_chroma;
}

void UIWaitVisitorInfo::UserAgent(QString val)
{
    m_userAgent = val;
}

QString UIWaitVisitorInfo::UserAgent() const
{
    return m_userAgent;
}
void UIWaitVisitorInfo::Ie(QString val)
{
    m_ie = val;
}

QString UIWaitVisitorInfo::Ie() const
{
    return m_ie;
}
