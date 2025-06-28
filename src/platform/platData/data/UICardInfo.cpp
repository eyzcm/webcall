#include "UICardInfo.h"
#include <QUrl>
#include "utils/constDefine.h"

UICardInfo::UICardInfo(void)
{
    Id("");
}

UICardInfo::UICardInfo(const UICardInfo& info)
{
    this->Area(info.Area());
    this->ChatURL(info.ChatURL());
    this->CompanyId(info.CompanyId());
    this->CompanyName(info.CompanyName());
    this->CreateTime(info.CreateTime());
    this->CreateUserId(info.CreateUserId());
    this->EditTime(info.EditTime());
    this->EditUserId(info.EditUserId());
    this->Email(info.Email());
    this->Tag(info.Tag());
    this->Ip(info.Ip());
    this->Ipv6(info.Ipv6());
    this->ExtColumn1(info.ExtColumn1());
    this->ExtColumn10(info.ExtColumn10());
    this->ExtColumn2(info.ExtColumn2());
    this->ExtColumn3(info.ExtColumn3());
    this->ExtColumn4(info.ExtColumn4());
    this->ExtColumn5(info.ExtColumn5());
    this->ExtColumn6(info.ExtColumn6());
    this->ExtColumn7(info.ExtColumn7());
    this->ExtColumn8(info.ExtColumn8());
    this->ExtColumn9(info.ExtColumn9());
    this->ExtColumn11(info.ExtColumn11());
    this->ExtColumn12(info.ExtColumn12());
    this->ExtColumn13(info.ExtColumn13());
    this->ExtColumn14(info.ExtColumn14());
    this->ExtColumn15(info.ExtColumn15());
    this->FirstURL(info.FirstURL());
    this->Id(info.Id());
    this->KeyWord(info.KeyWord());
    this->Mobile(info.Mobile());
    this->PhoneStatus(info.PhoneStatus());
    this->Msn(info.Msn());
    this->Name(info.Name());
    this->Note(info.Note());
    this->PromotionId(info.PromotionId());
    this->PromotionName(info.PromotionName());
    this->Qq(info.Qq());
    this->RepName(info.RepName());
    this->ReseveKey(info.ReseveKey());
    this->SearchEngine(info.SearchEngine());
    this->SearchHost(info.SearchHost());
    this->Sex(info.Sex());
    this->SpreadFlag(info.SpreadFlag());
    this->Tel(info.Tel());
    this->Url(info.Url());
    this->UserId(info.UserId());
    this->VisitorStaticId(info.VisitorStaticId());

    this->VisitorLocationCity(info.VisitorLocationCity());
    this->VisitorLocationCountry(info.VisitorLocationCountry());
    this->VisitorLocationProvince(info.VisitorLocationProvince());
    this->VisitorLocationDistrict(info.VisitorLocationDistrict());
    this->SiteId(info.SiteId());
    this->ChatType(info.ChatType());
    this->ChatId(info.ChatId());
    this->GroupId(info.GroupId());
    this->Refer(info.Refer());
    this->Ssid(info.Ssid());

    this->ExtendKeyword(info.ExtendKeyword());
    this->BcpBiddingWord(info.BcpBiddingWord());
}

UICardInfo::~UICardInfo(void)
{
}

QMap<QString, QString> UICardInfo::ToMap() const
{
    QMap<QString, QString> ret;
    ret.insert("companyName", CompanyName());
    ret.insert("area", Area());
    ret.insert("email", Email());
    ret.insert("mobile", Mobile());
    ret.insert("phoneStatus", PhoneStatus());
    ret.insert("msn", Msn());
    ret.insert("name", Name());
    ret.insert("note", Note());
    ret.insert("qq", Qq());
    ret.insert("repName", RepName());
    ret.insert("searchEngine", SearchEngine());
    ret.insert("sex", Sex());
    ret.insert("tel", Tel());
    ret.insert("extColumn1", ExtColumn1());
    ret.insert("extColumn2", ExtColumn2());
    ret.insert("extColumn3", ExtColumn3());
    ret.insert("extColumn4", ExtColumn4());
    ret.insert("extColumn5", ExtColumn5());
    ret.insert("extColumn6", ExtColumn6());
    ret.insert("extColumn7", ExtColumn7());
    ret.insert("extColumn8", ExtColumn8());
    ret.insert("extColumn9", ExtColumn9());
    ret.insert("extColumn10", ExtColumn10());
    ret.insert("extColumn11", ExtColumn11());
    ret.insert("extColumn12", ExtColumn12());
    ret.insert("extColumn13", ExtColumn13());
    ret.insert("extColumn14", ExtColumn14());
    ret.insert("extColumn15", ExtColumn15());

    ret.insert("ntag", Tag());
    ret.insert("url", Url());
    ret.insert("keyWord", KeyWord());
    ret.insert("chatURL", ChatURL());
    ret.insert("firstURL", FirstURL());
    ret.insert("ip", Ip());
    ret.insert("ipv6", Ipv6());
    ret.insert("ssid", Ssid());

    /*
    ret.insert("chatId" ,ChatId());
    ret.insert("ntag",Tag());
    ret.insert("promotionId" ,PromotionId());
    ret.insert("PromotionName" ,PromotionName());
    ret.insert("searchHost" ,SearchHost());
    ret.insert("spreadFlag" ,SpreadFlag());
    ret.insert("url" ,Url());
    ret.insert("visitorStaticId" ,VisitorStaticId());
    ret.insert("ip" ,Ip());
    ret.insert("ipv6" ,Ipv6());
    ret.insert("firstURL" ,FirstURL());
    ret.insert("keyWord" ,KeyWord());
    ret.insert("chatURL" ,ChatURL());
    ret.insert("visitor_location_country" ,VisitorLocationCountry());
    ret.insert("visitor_location_province" ,VisitorLocationProvince());
    ret.insert("visitor_location_city" ,VisitorLocationCity());
    ret.insert("siteId" ,SiteId());
    ret.insert("chat_type" ,ChatType());
    ret.insert("refer" ,Refer());
    ret.insert("lastLandingPage" ,this->LastLandingPage());
    */
    return ret;
}


QString UICardInfo::ToString(QMap<QString,QString> map) const
{


    return BaseToString(map);
}

QString UICardInfo::BaseToString(QMap<QString,QString> map) const
{
    QString ret = "";
    ret.append(QString("%1=%2&").arg("chatId").arg(UrlEncode(ChatId())));
    ret.append(QString("%1=%2&").arg("groupId").arg(UrlEncode(GroupId())));
    ret.append(QString("%1=%2&").arg("companyName").arg(UrlEncode(CompanyName())));
    ret.append(QString("%1=%2&").arg("area").arg(UrlEncode(Area())));
    ret.append(QString("%1=%2&").arg("email").arg(UrlEncode(Email())));
    ret.append(QString("%1=%2&").arg("mobile").arg(UrlEncode(Mobile())));
    ret.append(QString("%1=%2&").arg("phoneStatus").arg(UrlEncode(PhoneStatus())));
    ret.append(QString("%1=%2&").arg("msn").arg(UrlEncode(Msn())));
    //ret.append(QString("%1=%2&").arg("name").arg(UrlEncode(Name())));
    ret.append(QString("%1=%2&").arg("note").arg(UrlEncode(Note())));
    ret.append(QString("%1=%2&").arg("ntag").arg(Tag()));
    ret.append(QString("%1=%2&").arg("promotionId").arg(UrlEncode(PromotionId())));
    ret.append(QString("%1=%2&").arg("qq").arg(UrlEncode(Qq())));
    ret.append(QString("%1=%2&").arg("repName").arg(UrlEncode(RepName())));
    ret.append(QString("%1=%2&").arg("searchEngine").arg(UrlEncode(SearchEngine())));
    ret.append(QString("%1=%2&").arg("searchHost").arg(UrlEncode(SearchHost())));
    ret.append(QString("%1=%2&").arg("sex").arg(UrlEncode(Sex())));
    ret.append(QString("%1=%2&").arg("spreadFlag").arg(UrlEncode(SpreadFlag())));
    ret.append(QString("%1=%2&").arg("tel").arg(UrlEncode(Tel())));
    ret.append(QString("%1=%2&").arg("url").arg(UrlEncode(Url())));
    ret.append(QString("%1=%2&").arg("visitorStaticId").arg(UrlEncode(VisitorStaticId())));
    if( (Name().contains( "未备注"))&&( ( (map.size() > 0) && (map.find("name") != map.end() )&&(map["name"] != ""))))
    {
        ret.append(QString("%1=%2&").arg("name").arg(UrlEncode(map["name"])));
    }else
        ret.append(QString("%1=%2&").arg("name").arg(UrlEncode(Name())));


    if( (ExtColumn1() == "")&&( ( (map.size() > 0) && (map.find("extColumn1") != map.end() )&&(map["extColumn1"] != ""))))
        ret.append(QString("%1=%2&").arg("extColumn1").arg(UrlEncode(map["extColumn1"])));
    else
        ret.append(QString("%1=%2&").arg("extColumn1").arg(UrlEncode(ExtColumn1())));

    if( (ExtColumn2() == "")&&( ( (map.size() > 0) && (map.find("extColumn2") != map.end() )&&(map["extColumn2"] != ""))))
        ret.append(QString("%1=%2&").arg("extColumn2").arg(UrlEncode(map["extColumn2"])));
    else
        ret.append(QString("%1=%2&").arg("extColumn2").arg(UrlEncode(ExtColumn2())));
    //ret.append(QString("%1=%2&").arg("extColumn2").arg(UrlEncode(ExtColumn2())));
    ret.append(QString("%1=%2&").arg("extColumn3").arg(UrlEncode(ExtColumn3())));
    ret.append(QString("%1=%2&").arg("extColumn4").arg(UrlEncode(ExtColumn4())));
    ret.append(QString("%1=%2&").arg("extColumn5").arg(UrlEncode(ExtColumn5())));
    ret.append(QString("%1=%2&").arg("extColumn6").arg(UrlEncode(ExtColumn6())));
    ret.append(QString("%1=%2&").arg("extColumn7").arg(UrlEncode(ExtColumn7())));
    if( (map.size() > 0) && (map.find("extColumn8") != map.end() )&&(map["extColumn8"] != ""))
        ret.append(QString("%1=%2&").arg("extColumn8").arg(UrlEncode(map["extColumn8"])));
    else
        ret.append(QString("%1=%2&").arg("extColumn8").arg(UrlEncode(ExtColumn8())));
    if( (map.size() > 0) && (map.find("extColumn9") != map.end() )&&(map["extColumn9"] != ""))
            ret.append(QString("%1=%2&").arg("extColumn9").arg(UrlEncode(map["extColumn9"])));
        else
            ret.append(QString("%1=%2&").arg("extColumn9").arg(UrlEncode(ExtColumn9())));
    if( (map.size() > 0) && (map.find("extColumn10") != map.end() )&&(map["extColumn10"] != ""))
            ret.append(QString("%1=%2&").arg("extColumn10").arg(UrlEncode(map["extColumn10"])));
    else
        ret.append(QString("%1=%2&").arg("extColumn10").arg(UrlEncode(ExtColumn10())));

    ret.append(QString("%1=%2&").arg("extColumn11").arg(UrlEncode(ExtColumn11())));
    ret.append(QString("%1=%2&").arg("extColumn12").arg(UrlEncode(ExtColumn12())));
    ret.append(QString("%1=%2&").arg("extColumn13").arg(UrlEncode(ExtColumn13())));
    ret.append(QString("%1=%2&").arg("extColumn14").arg(UrlEncode(ExtColumn14())));
    ret.append(QString("%1=%2&").arg("extColumn15").arg(UrlEncode(ExtColumn15())));
    ret.append(QString("%1=%2&").arg("ip").arg(UrlEncode(Ip())));
    ret.append(QString("%1=%2&").arg("ipv6").arg(UrlEncode(Ipv6())));

    ret.append(QString("%1=%2&").arg("firstURL").arg(UrlEncode(FirstURL())));
    ret.append(QString("%1=%2&").arg("keyWord").arg(UrlEncode(KeyWord())));
    ret.append(QString("%1=%2&").arg("chatURL").arg(UrlEncode(ChatURL())));

    ret.append(QString("%1=%2&").arg("visitor_location_country").arg(UrlEncode(VisitorLocationCountry())));
    ret.append(QString("%1=%2&").arg("visitor_location_province").arg(UrlEncode(VisitorLocationProvince())));
    ret.append(QString("%1=%2&").arg("visitor_location_city").arg(UrlEncode(VisitorLocationCity())));
    ret.append(QString("%1=%2&").arg("visitor_location_district").arg(UrlEncode(VisitorLocationDistrict())));
    ret.append(QString("%1=%2&").arg("siteId").arg(UrlEncode(SiteId())));
    ret.append(QString("%1=%2&").arg("chat_type").arg(UrlEncode(ChatType())));
    ret.append(QString("%1=%2&").arg("refer").arg(UrlEncode(Refer())));
    ret.append(QString("%1=%2&").arg("lastLandingPage").arg(UrlEncode(this->LastLandingPage())));
    ret.append(QString("%1=%2&").arg("ssid").arg(UrlEncode(this->Ssid())));
    // todo 9814
    ret.append(QString("%1=%2&").arg(BCP_BIDDING_WORD).arg(UrlEncode(BcpBiddingWord())));
    ret.append(QString("%1=%2").arg(EXTEND_KEYWORD).arg(UrlEncode(ExtendKeyword())));

    return ret;
}

void UICardInfo::Area(::QString val)
{
    m_area = val;
}

::QString UICardInfo::Area() const
{
    return m_area;
}

void UICardInfo::ChatURL(::QString val)
{
    if (m_chatURL.isEmpty() && !val.isEmpty())
    {
        m_chatURL = val;
    }
}

::QString UICardInfo::ChatURL() const
{
    return m_chatURL;
}

void UICardInfo::CompanyId(::QString val)
{
    m_companyId = val;
}

::QString UICardInfo::CompanyId() const
{
    return m_companyId;
}

void UICardInfo::CompanyName(::QString val)
{
    m_companyName = val;
}

::QString UICardInfo::CompanyName() const
{
    return m_companyName;
}

void UICardInfo::CreateTime(::QString val)
{
    m_createTime = val;
}

::QString UICardInfo::CreateTime() const
{
    return m_createTime;
}

void UICardInfo::Email(::QString val)
{
    m_email = val;
}

::QString UICardInfo::Email() const
{
    return m_email;
}

void UICardInfo::FirstURL(::QString val)
{
    if (m_firstURL.isEmpty() && !val.isEmpty())
    {
        m_firstURL = val;
    }
}

::QString UICardInfo::FirstURL() const
{
    return m_firstURL;
}

void UICardInfo::Id(::QString val)
{
    m_id = val;
}

::QString UICardInfo::Id() const
{
    return m_id;
}

void UICardInfo::KeyWord(::QString val)
{
    if (m_keyWord.isEmpty() && !val.isEmpty())
    {
        m_keyWord = val;
    }
}

::QString UICardInfo::KeyWord() const
{
    return m_keyWord;
}

void UICardInfo::Mobile(::QString val)
{
    m_mobile = val;
}

::QString UICardInfo::Mobile() const
{
    return m_mobile;
}

void UICardInfo::PhoneStatus(QString val)
{
    m_phoneStatus = val;
}

QString UICardInfo::PhoneStatus() const
{
    return m_phoneStatus;
}


void UICardInfo::Msn(::QString val)
{
    m_msn = val;
}

::QString UICardInfo::Msn() const
{
    return m_msn;
}

void UICardInfo::Name(::QString val)
{
    m_name = val;
}

::QString UICardInfo::Name() const
{
    return m_name;
}

void UICardInfo::Note(::QString val)
{
    m_note = val;
}

::QString UICardInfo::Note() const
{
    return m_note;
}

void UICardInfo::PromotionId(::QString val)
{
    if (m_promotionId.isEmpty() && !val.isEmpty())
    {
        m_promotionId = val;
    }
}

::QString UICardInfo::PromotionId() const
{
    return m_promotionId;
}


void UICardInfo::PromotionName(::QString val)
{
    if (m_promotionName.isEmpty() && !val.isEmpty())
    {
        m_promotionName = val;
    }
}

::QString UICardInfo::PromotionName() const
{
    return m_promotionName;
}



void UICardInfo::Qq(::QString val)
{
    m_qq = val;
}

::QString UICardInfo::Qq() const
{
    return m_qq;
}

void UICardInfo::RepName(::QString val)
{
    m_repName = val;
}

::QString UICardInfo::RepName() const
{
    return m_repName;
}

void UICardInfo::ReseveKey(::QString val)
{
    m_reseveKey = val;
}

::QString UICardInfo::ReseveKey() const
{
    return m_reseveKey;
}

void UICardInfo::SearchEngine(::QString val)
{
    if (m_searchEngine.isEmpty() && !val.isEmpty())
    {
        m_searchEngine = val;
    }
}

::QString UICardInfo::SearchEngine() const
{
    return m_searchEngine;
}

void UICardInfo::SearchHost(::QString val)
{
    if (m_searchHost.isEmpty() && !val.isEmpty())
    {
        m_searchHost = val;
    }
}

::QString UICardInfo::SearchHost() const
{
    return m_searchHost;
}

void UICardInfo::Sex(::QString val)
{
    m_sex = val;
}

::QString UICardInfo::Sex() const
{
    return m_sex;
}

void UICardInfo::SpreadFlag(::QString val)
{
    if (m_spreadFlag.isEmpty() && !val.isEmpty())
    {
        m_spreadFlag = val;
    }
}

::QString UICardInfo::SpreadFlag() const
{
    return m_spreadFlag;
}

void UICardInfo::Tel(::QString val)
{
    m_tel = val;
}

::QString UICardInfo::Tel() const
{
    return m_tel;
}

void UICardInfo::Url(::QString val)
{
    m_url = val;
}

::QString UICardInfo::Url() const
{
    return m_url;
}

void UICardInfo::UserId(::QString val)
{
    m_userId = val;
}

::QString UICardInfo::UserId() const
{
    return m_userId;
}

void UICardInfo::VisitorStaticId(::QString val)
{
    m_visitorStaticId = val;
}

::QString UICardInfo::VisitorStaticId() const
{
    return m_visitorStaticId;
}

void UICardInfo::ExtColumn1(::QString val)
{
    m_extColumn1 = val;
}

::QString UICardInfo::ExtColumn1() const
{
    return m_extColumn1;
}

void UICardInfo::ExtColumn10(::QString val)
{
    m_extColumn10 = val;
}

::QString UICardInfo::ExtColumn10() const
{
    return m_extColumn10;
}

void UICardInfo::ExtColumn2(::QString val)
{
    m_extColumn2 = val;
}

::QString UICardInfo::ExtColumn2() const
{
    return m_extColumn2;
}

void UICardInfo::ExtColumn3(::QString val)
{
    m_extColumn3 = val;
}

::QString UICardInfo::ExtColumn3() const
{
    return m_extColumn3;
}

void UICardInfo::ExtColumn4(::QString val)
{
    m_extColumn4 = val;
}

::QString UICardInfo::ExtColumn4() const
{
    return m_extColumn4;
}

void UICardInfo::ExtColumn5(::QString val)
{
    m_extColumn5 = val;
}

::QString UICardInfo::ExtColumn5() const
{
    return m_extColumn5;
}

void UICardInfo::ExtColumn6(::QString val)
{
    m_extColumn6 = val;
}

::QString UICardInfo::ExtColumn6() const
{
    return m_extColumn6;
}

void UICardInfo::ExtColumn7(::QString val)
{
    m_extColumn7 = val;
}

::QString UICardInfo::ExtColumn7() const
{
    return m_extColumn7;
}

void UICardInfo::ExtColumn8(::QString val)
{
    m_extColumn8 = val;
}

::QString UICardInfo::ExtColumn8() const
{
    return m_extColumn8;
}

void UICardInfo::ExtColumn9(::QString val)
{
    m_extColumn9 = val;
}

::QString UICardInfo::ExtColumn9() const
{
    return m_extColumn9;
}

void UICardInfo::CreateUserId(QString val)
{
    m_createUserId = val;
}

QString UICardInfo::CreateUserId() const
{
    return m_createUserId;
}

void UICardInfo::EditTime(QString val)
{
    m_editTime = val;
}

QString UICardInfo::EditTime() const
{
    return m_editTime;
}

void UICardInfo::EditUserId(QString val)
{
    m_editUserId = val;
}

QString UICardInfo::EditUserId() const
{
    return m_editUserId;
}

QString UICardInfo::GetValueByKey(QString strKey) const
{
    if (strKey.compare("area", Qt::CaseInsensitive) == 0)
    {
        return m_area;
    }
    else if (strKey.compare("chatURL", Qt::CaseInsensitive) == 0)
    {
        return m_chatURL;
    }
    else if (strKey.compare("companyId", Qt::CaseInsensitive) == 0)
    {
        return m_companyId;
    }
    else if (strKey.compare("companyName", Qt::CaseInsensitive) == 0)
    {
        return m_companyName;
    }
    else if (strKey.compare("createTime", Qt::CaseInsensitive) == 0)
    {
        return m_createTime;
    }
    else if (strKey.compare("email", Qt::CaseInsensitive) == 0)
    {
        return m_email;
    }
    else if (strKey.compare("extColumn1", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn1;
    }
    else if (strKey.compare("extColumn10", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn10;
    }
    else if (strKey.compare("extColumn2", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn2;
    }
    else if (strKey.compare("extColumn3", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn3;
    }
    else if (strKey.compare("extColumn4", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn4;
    }
    else if (strKey.compare("extColumn5", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn5;
    }
    else if (strKey.compare("extColumn6", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn6;
    }
    else if (strKey.compare("extColumn7", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn7;
    }
    else if (strKey.compare("extColumn8", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn8;
    }
    else if (strKey.compare("extColumn9", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn9;
    }
    else if (strKey.compare("extColumn11", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn11;
    }
    else if (strKey.compare("extColumn12", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn12;
    }
    else if (strKey.compare("extColumn13", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn13;
    }
    else if (strKey.compare("extColumn14", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn14;
    }
    else if (strKey.compare("extColumn15", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn15;
    }
    else if (strKey.compare("ip", Qt::CaseInsensitive) == 0)
    {
        return m_ip;
    }
    else if (strKey.compare("ipv6", Qt::CaseInsensitive) == 0)
    {
        return m_ipv6;
    }
    else if (strKey.compare("extColumn11", Qt::CaseInsensitive) == 0)
    {
        return m_extColumn11;
    }
    else if (strKey.compare("firstURL", Qt::CaseInsensitive) == 0)
    {
        return m_firstURL;
    }
    else if (strKey.compare("id", Qt::CaseInsensitive) == 0)
    {
        return m_id;
    }
    else if (strKey.compare("keyWord", Qt::CaseInsensitive) == 0)
    {
        return m_keyWord;
    }
    else if (strKey.compare("mobile", Qt::CaseInsensitive) == 0)
    {
        return m_mobile;
    }
    else if (strKey.compare("phoneStatus", Qt::CaseInsensitive) == 0)
    {
        return m_phoneStatus;
    }
    else if (strKey.compare("msn", Qt::CaseInsensitive) == 0)
    {
        return m_msn;
    }
    else if (strKey.compare("name", Qt::CaseInsensitive) == 0)
    {
        return m_name;
    }
    else if (strKey.compare("note", Qt::CaseInsensitive) == 0)
    {
        return m_note;
    }
    else if (strKey.compare("promotionId", Qt::CaseInsensitive) == 0)
    {
        return m_promotionId;
    }
    else if (strKey.compare("qq", Qt::CaseInsensitive) == 0)
    {
        return m_qq;
    }
    else if (strKey.compare("ntag", Qt::CaseInsensitive) == 0)
    {
        return m_nTag;
    }
    else if (strKey.compare("repName", Qt::CaseInsensitive) == 0)
    {
        return m_repName;
    }
    else if (strKey.compare("searchEngine", Qt::CaseInsensitive) == 0)
    {
        return m_searchEngine;
    }
    else if (strKey.compare("searchHost", Qt::CaseInsensitive) == 0)
    {
        return m_searchHost;
    }
    else if (strKey.compare("sex", Qt::CaseInsensitive) == 0)
    {
        return m_sex;
    }
    else if (strKey.compare("spreadFlag", Qt::CaseInsensitive) == 0)
    {
        return m_spreadFlag;
    }
    else if (strKey.compare("tel", Qt::CaseInsensitive) == 0)
    {
        return m_tel;
    }
    else if (strKey.compare("url", Qt::CaseInsensitive) == 0)
    {
        return m_url;
    }
    else if (strKey.compare("userId", Qt::CaseInsensitive) == 0)
    {
        return m_userId;
    }
    else if (strKey.compare("visitorStaticId", Qt::CaseInsensitive) == 0)
    {
        return m_visitorStaticId;
    }
    else if (strKey.compare("reseveKey", Qt::CaseInsensitive) == 0)
    {
        return m_reseveKey;
    }
    else if (strKey.compare("createUserId", Qt::CaseInsensitive) == 0)
    {
        return m_createUserId;
    }
    else if (strKey.compare("editTime", Qt::CaseInsensitive) == 0)
    {
        return m_editTime;
    }
    else if (strKey.compare("editUserId", Qt::CaseInsensitive) == 0)
    {
        return m_editUserId;
    }
    else if (strKey.compare("createTime", Qt::CaseInsensitive) == 0)
    {
        return m_createTime;
    }
    else if (strKey.compare("refer", Qt::CaseInsensitive) == 0)
    {
        return m_refer;
    }
    else if (strKey.compare("chatId", Qt::CaseInsensitive) == 0)
    {
        return m_chatId;
    }
    else if (strKey.compare("ocpc", Qt::CaseInsensitive) == 0)
    {
        return m_ocpcData;
    }
    else if (strKey.compare("ssid", Qt::CaseInsensitive) == 0)
    {
        return m_ssid;
    }
    else if (strKey.compare(BCP_BIDDING_WORD, Qt::CaseInsensitive) == 0)
    {
        return m_bcpBiddingWord;
    }
    else if (strKey.compare(EXTEND_KEYWORD, Qt::CaseInsensitive) == 0)
    {
        return m_extendKeyword;
    }
    else
    {
        // Q_ASSERT(false);
        return "";
    }
}

void UICardInfo::SetValueByKey(QString strKey, QString strVal)
{
    if (strKey.compare("area", Qt::CaseInsensitive) == 0)
    {
        Area(strVal);
    }
    else if (strKey.compare("chatURL", Qt::CaseInsensitive) == 0)
    {
        ChatURL(strVal);
    }
    else if (strKey.compare("companyId", Qt::CaseInsensitive) == 0)
    {
        CompanyId(strVal);
    }
    else if (strKey.compare("companyName", Qt::CaseInsensitive) == 0)
    {
        CompanyName(strVal);
    }
    else if (strKey.compare("email", Qt::CaseInsensitive) == 0)
    {
        Email(strVal);
    }
    else if (strKey.compare("ntag", Qt::CaseInsensitive) == 0)
    {
        Tag(strVal);
    }
    else if (strKey.compare("extColumn1", Qt::CaseInsensitive) == 0)
    {
        ExtColumn1(strVal);
    }
    else if (strKey.compare("extColumn10", Qt::CaseInsensitive) == 0)
    {
        ExtColumn10(strVal);
    }
    else if (strKey.compare("extColumn2", Qt::CaseInsensitive) == 0)
    {
        ExtColumn2(strVal);
    }
    else if (strKey.compare("extColumn3", Qt::CaseInsensitive) == 0)
    {
        ExtColumn3(strVal);
    }
    else if (strKey.compare("extColumn4", Qt::CaseInsensitive) == 0)
    {
        ExtColumn4(strVal);
    }
    else if (strKey.compare("extColumn5", Qt::CaseInsensitive) == 0)
    {
        ExtColumn5(strVal);
    }
    else if (strKey.compare("extColumn6", Qt::CaseInsensitive) == 0)
    {
        ExtColumn6(strVal);
    }
    else if (strKey.compare("extColumn7", Qt::CaseInsensitive) == 0)
    {
        ExtColumn7(strVal);
    }
    else if (strKey.compare("extColumn8", Qt::CaseInsensitive) == 0)
    {
        ExtColumn8(strVal);
    }
    else if (strKey.compare("extColumn9", Qt::CaseInsensitive) == 0)
    {
        ExtColumn9(strVal);
    }
    else if (strKey.compare("extColumn11", Qt::CaseInsensitive) == 0)
    {
        ExtColumn11(strVal);
    }
    else if (strKey.compare("extColumn12", Qt::CaseInsensitive) == 0)
    {
        ExtColumn12(strVal);
    }
    else if (strKey.compare("extColumn13", Qt::CaseInsensitive) == 0)
    {
        ExtColumn13(strVal);
    }
    else if (strKey.compare("extColumn14", Qt::CaseInsensitive) == 0)
    {
        ExtColumn14(strVal);
    }
    else if (strKey.compare("extColumn15", Qt::CaseInsensitive) == 0)
    {
        ExtColumn15(strVal);
    }
    else if (strKey.compare("ip", Qt::CaseInsensitive) == 0)
    {
        Ip(strVal);
    }
    else if (strKey.compare("ipv6", Qt::CaseInsensitive) == 0)
    {
        Ipv6(strVal);
    }
    else if (strKey.compare("firstURL", Qt::CaseInsensitive) == 0)
    {
        FirstURL(strVal);
    }
    else if (strKey.compare("keyWord", Qt::CaseInsensitive) == 0)
    {
        KeyWord(strVal);
    }
    else if (strKey.compare("mobile", Qt::CaseInsensitive) == 0)
    {
        Mobile(strVal);
    }
    else if (strKey.compare("phoneStatus", Qt::CaseInsensitive) == 0)
    {
        PhoneStatus(strVal);
    }
    else if (strKey.compare("msn", Qt::CaseInsensitive) == 0)
    {
        Msn(strVal);
    }
    else if (strKey.compare("name", Qt::CaseInsensitive) == 0)
    {
        Name(strVal);
    }
    else if (strKey.compare("note", Qt::CaseInsensitive) == 0)
    {
        Note(strVal);
    }
    else if (strKey.compare("promotionId", Qt::CaseInsensitive) == 0)
    {
        PromotionId(strVal);
    }
    else if (strKey.compare("qq", Qt::CaseInsensitive) == 0)
    {
        Qq(strVal);
    }
    else if (strKey.compare("repName", Qt::CaseInsensitive) == 0)
    {
        RepName(strVal);
    }
    else if (strKey.compare("searchEngine", Qt::CaseInsensitive) == 0)
    {
        SearchEngine(strVal);
    }
    else if (strKey.compare("searchHost", Qt::CaseInsensitive) == 0)
    {
        SearchHost(strVal);
    }
    else if (strKey.compare("sex", Qt::CaseInsensitive) == 0)
    {
        Sex(strVal);
    }
    else if (strKey.compare("spreadFlag", Qt::CaseInsensitive) == 0)
    {
        SpreadFlag(strVal);
    }
    else if (strKey.compare("tel", Qt::CaseInsensitive) == 0)
    {
        Tel(strVal);
    }
    else if (strKey.compare("url", Qt::CaseInsensitive) == 0)
    {
        Url(strVal);
    }
    else if (strKey.compare("userId", Qt::CaseInsensitive) == 0)
    {
        UserId(strVal);
    }
    else if (strKey.compare("visitorStaticId", Qt::CaseInsensitive) == 0)
    {
        VisitorStaticId(strVal);
    }
    else if (strKey.compare("createUserId", Qt::CaseInsensitive) == 0)
    {
        CreateUserId(strVal);
    }
    else if (strKey.compare("editTime", Qt::CaseInsensitive) == 0)
    {
        EditTime(strVal);
    }
    else if (strKey.compare("editUserId", Qt::CaseInsensitive) == 0)
    {
        EditUserId(strVal);
    }
    else if (strKey.compare("createTime", Qt::CaseInsensitive) == 0)
    {
        CreateTime(strVal);
    }
    else if (strKey.compare("refer", Qt::CaseInsensitive) == 0)
    {
        Refer(strVal);
    }
    else if (strKey.compare("chatId", Qt::CaseInsensitive) == 0)
    {
        ChatId(strVal);
    }
    else if (strKey.compare("siteId", Qt::CaseInsensitive) == 0)
    {
        SiteId(strVal);
    }
    else if (strKey.compare("chat_type", Qt::CaseInsensitive) == 0)
    {
        ChatType(strVal);
    }
    else if (strKey.compare("visitor_location_city", Qt::CaseInsensitive) == 0)
    {
        VisitorLocationCity(strVal);
    }
    else if (strKey.compare("visitor_location_district", Qt::CaseInsensitive) == 0)
    {
        VisitorLocationDistrict(strVal);
    }
    else if (strKey.compare("visitor_location_country", Qt::CaseInsensitive) == 0)
    {
        VisitorLocationCountry(strVal);
    }
    else if (strKey.compare("visitor_location_province", Qt::CaseInsensitive) == 0)
    {
        VisitorLocationProvince(strVal);
    }
    else if (strKey.compare("ssid", Qt::CaseInsensitive) == 0)
    {
        Ssid(strVal);
    }
    else if (strKey.compare(BCP_BIDDING_WORD, Qt::CaseInsensitive) == 0)
    {
        BcpBiddingWord(strVal);
    }
    else if (strKey.compare(EXTEND_KEYWORD, Qt::CaseInsensitive) == 0)
    {
        ExtendKeyword(strVal);
    }
    else
    {
        Q_ASSERT(false);
    }
}

QString UICardInfo::Refer() const
{
    return m_refer;
}

void UICardInfo::Refer(QString val)
{
    if (m_refer.isEmpty() && !val.isEmpty())
    {
        m_refer = val;
    }
}

QString UICardInfo::ChatId() const
{
    return m_chatId;
}

void UICardInfo::ChatId(QString val)
{
    m_chatId = val;
}

QString UICardInfo::UrlEncode(QString val) const
{
    QString ret = QUrl::toPercentEncoding(val, "", "?&");
    // QString ret = QUrl(val).toEncoded();
    return ret;
}

void UICardInfo::ChatType(QString val)
{
    if (m_chat_type.isEmpty() && !val.isEmpty())
    {
        m_chat_type = val;
    }
}

QString UICardInfo::ChatType() const
{
    return m_chat_type;
}

void UICardInfo::SiteId(QString val)
{
    if (m_siteId.isEmpty() && !val.isEmpty())
    {
        m_siteId = val;
    }
}

QString UICardInfo::SiteId() const
{
    return m_siteId;
}

void UICardInfo::VisitorLocationCity(QString val)
{
    if (m_visitor_location_city.isEmpty() && !val.isEmpty())
    {
        m_visitor_location_city = val;
    }
}

QString UICardInfo::VisitorLocationCity() const
{
    return m_visitor_location_city;
}

void UICardInfo::VisitorLocationDistrict(QString val)
{
    if (m_visitor_location_district.isEmpty() && !val.isEmpty())
    {
        m_visitor_location_district = val;
    }
}

QString UICardInfo::VisitorLocationDistrict() const
{
    return m_visitor_location_district;
}

void UICardInfo::VisitorLocationProvince(QString val)
{
    if (m_visitor_location_province.isEmpty() && !val.isEmpty())
    {
        m_visitor_location_province = val;
    }
}

QString UICardInfo::VisitorLocationProvince() const
{
    return m_visitor_location_province;
}

void UICardInfo::VisitorLocationCountry(QString val)
{
    if (m_visitor_location_country.isEmpty() && !val.isEmpty())
    {
        m_visitor_location_country = val;
    }
}

QString UICardInfo::VisitorLocationCountry() const
{
    return m_visitor_location_country;
}

bool UICardInfo::IsHasContact()
{
    if (!m_qq.isEmpty())
        return true;
    if (!m_mobile.isEmpty())
        return true;
    if (!m_tel.isEmpty())
        return true;
    if (!m_msn.isEmpty())
        return true;

    return false;
}

bool UICardInfo::IsAiEdit()
{
    if ((m_editUserId.length() > 3) && (m_editUserId.left(3).compare("AI-", Qt::CaseInsensitive) == 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString UICardInfo::ExtColumn11() const
{
    return m_extColumn11;
}

void UICardInfo::ExtColumn11(QString val)
{
    m_extColumn11 = val;
}

QString UICardInfo::ExtColumn12() const
{
    return m_extColumn12;
}

void UICardInfo::ExtColumn12(QString val)
{
    m_extColumn12 = val;
}

QString UICardInfo::ExtColumn13() const
{
    return m_extColumn13;
}

void UICardInfo::ExtColumn13(QString val)
{
    m_extColumn13 = val;
}

QString UICardInfo::ExtColumn14() const
{
    return m_extColumn14;
}

void UICardInfo::ExtColumn14(QString val)
{
    m_extColumn14 = val;
}

QString UICardInfo::ExtColumn15() const
{
    return m_extColumn15;
}

void UICardInfo::ExtColumn15(QString val)
{
    m_extColumn15 = val;
}

QString UICardInfo::Ip() const
{
    return m_ip;
}

void UICardInfo::Ip(QString val)
{
    m_ip = val;
}

QString UICardInfo::Ipv6() const
{
    return m_ipv6;
}

void UICardInfo::Ipv6(QString val)
{
    m_ipv6 = val;
}

QString UICardInfo::LastLandingPage() const
{
    return m_lastLandingPage;
}

void UICardInfo::LastLandingPage(QString val)
{
    m_lastLandingPage = val;
}
QString UICardInfo::GroupId() const
{
    return m_groupId;
}

void UICardInfo::GroupId(QString val)
{
    m_groupId = val;
}

// todo 9814
QString UICardInfo::BcpBiddingWord() const
{
    return m_bcpBiddingWord;
}

void UICardInfo::BcpBiddingWord(QString val)
{
    m_bcpBiddingWord = val;
}

QString UICardInfo::ExtendKeyword() const
{
    return m_extendKeyword;
}

void UICardInfo::ExtendKeyword(QString val)
{
    m_extendKeyword = val;
}
