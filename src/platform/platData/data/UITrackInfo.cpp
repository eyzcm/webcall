#include "UITrackInfo.h"

UITrackInfo::UITrackInfo(void)
{
    m_isEndBrowser = false;
}

UITrackInfo::UITrackInfo(const UITrackInfo& info)
{
    this->ActiveTime(info.ActiveTime());
    this->CompanyId(info.CompanyId());
    this->CreateTime(info.CreateTime());
    this->HtmlName(info.HtmlName());
    this->Id(info.Id());
    this->KeyWord(info.KeyWord());
    this->PromotionId(info.PromotionId());
    this->PromotionName(info.PromotionName());
    this->Refer(info.Refer());
    this->SearchEngineId(info.SearchEngineId());
    this->Searching(info.Searching());
    this->SiteId(info.SiteId());
    this->SpreadFlag(info.SpreadFlag());
    this->Url(info.Url());
    this->StaticId(info.StaticId());
    this->ViewerId(info.ViewerId());
    this->IsEndBrowser(info.IsEndBrowser());
}

UITrackInfo::UITrackInfo(UITrackInfo* info)
{
    if (info == nullptr)
        return;
    this->ActiveTime(info->ActiveTime());
    this->CompanyId(info->CompanyId());
    this->CreateTime(info->CreateTime());
    this->HtmlName(info->HtmlName());
    this->Id(info->Id());
    this->KeyWord(info->KeyWord());
    this->PromotionId(info->PromotionId());
    this->PromotionName(info->PromotionName());
    this->Refer(info->Refer());
    this->SearchEngineId(info->SearchEngineId());
    this->Searching(info->Searching());
    this->SiteId(info->SiteId());
    this->SpreadFlag(info->SpreadFlag());
    this->Url(info->Url());
    this->StaticId(info->StaticId());
    this->ViewerId(info->ViewerId());
    this->IsEndBrowser(info->IsEndBrowser());
}

UITrackInfo::~UITrackInfo(void)
{
}

void UITrackInfo::ActiveTime(QString val)
{
    m_activeTime = val;
}

QString UITrackInfo::ActiveTime() const
{
    return m_activeTime;
}

void UITrackInfo::CompanyId(QString val)
{
    m_companyId = val;
}

QString UITrackInfo::CompanyId() const
{
    return m_companyId;
}

void UITrackInfo::CreateTime(QString val)
{
    m_createTime = val;
}

QString UITrackInfo::CreateTime() const
{
    return m_createTime;
}

void UITrackInfo::HtmlAlias(QString val)
{
    m_htmlAlias = val;
}

QString UITrackInfo::HtmlAlias() const
{
    return m_htmlAlias;
}

void UITrackInfo::HtmlName(QString val)
{
    m_htmlName = val;
}

QString UITrackInfo::HtmlName() const
{
    return m_htmlName;
}

void UITrackInfo::Id(QString val)
{
    m_id = val;
}

QString UITrackInfo::Id() const
{
    return m_id;
}

void UITrackInfo::KeyWord(QString val)
{
    m_keyWord = val;
}

QString UITrackInfo::KeyWord() const
{
    return m_keyWord;
}

void UITrackInfo::PromotionId(QString val)
{
    m_promotionId = val;
}

QString UITrackInfo::PromotionId() const
{
    return m_promotionId;
}

void UITrackInfo::PromotionName(QString val)
{
    m_promotionName = val;
}

QString UITrackInfo::PromotionName() const
{
    return m_promotionName;
}

void UITrackInfo::Refer(QString val)
{
    m_refer = val;
}

QString UITrackInfo::Refer() const
{
    return m_refer;
}

void UITrackInfo::SearchEngineId(QString val)
{
    m_searchEngineId = val;
}

QString UITrackInfo::SearchEngineId() const
{
    return m_searchEngineId;
}

void UITrackInfo::Searching(QString val)
{
    m_searching = val;
}

QString UITrackInfo::Searching() const
{
    return m_searching;
}

void UITrackInfo::SiteId(QString val)
{
    m_siteId = val;
}

QString UITrackInfo::SiteId() const
{
    return m_siteId;
}

void UITrackInfo::SpreadFlag(QString val)
{
    m_spreadFlag = val;
}

QString UITrackInfo::SpreadFlag() const
{
    return m_spreadFlag;
}

void UITrackInfo::Url(QString val)
{
    m_url = val;
}

QString UITrackInfo::Url() const
{
    return m_url;
}

void UITrackInfo::StaticId(QString val)
{
    m_userId = val;
}

QString UITrackInfo::StaticId() const
{
    return m_userId;
}

void UITrackInfo::ViewerId(QString val)
{
    m_viewerId = val;
}

QString UITrackInfo::ViewerId() const
{
    return m_viewerId;
}
