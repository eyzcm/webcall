#include "UIChatUserInfo.h"

#include "utils/utilshelper.h"

UIChatUserInfo::UIChatUserInfo(void)
{
    m_otherInfos = nullptr;
    m_cardInfo   = nullptr;
}

UIChatUserInfo::UIChatUserInfo(const UIChatUserInfo& info)
{

    AutoAccept(info.AutoAccept());
    IsWatcher(info.IsWatcher());
    ReseveKey(info.ReseveKey());
    UsePhoneMsg(info.UsePhoneMsg());
    UserId(info.UserId());
    UserType(info.UserType());
    StaticId(info.StaticId());
    Watcher(info.Watcher());
    setchatidlist(info.chatidlist());

    if (info.OtherInfos())
    {
        m_otherInfos = new CommInfo(*info.OtherInfos());
    }
    else
    {
        m_otherInfos = NULL;
    }
    info.OtherInfos();

    if (info.CardInfo())
    {
        m_cardInfo = new UICardInfo(*info.CardInfo());
    }
    else
    {
        m_cardInfo = NULL;
    }
}

UIChatUserInfo::~UIChatUserInfo(void)
{
    if (m_otherInfos)
    {
        delete m_otherInfos;
        m_otherInfos = nullptr;
    }
    if( m_cardInfo )
    {
        delete  m_cardInfo;
        m_cardInfo = nullptr;
    }
}

void UIChatUserInfo::AutoAccept(QString val)
{
    m_autoAccept = val;
}

QString UIChatUserInfo::AutoAccept() const
{
    return m_autoAccept;
}

void UIChatUserInfo::Watcher(QString val)
{
    m_watcher = val;
}

QString UIChatUserInfo::Watcher() const
{
    return m_watcher;
}

void UIChatUserInfo::ReseveKey(QString val)
{
    m_reseveKey = val;
}

QString UIChatUserInfo::ReseveKey() const
{
    return m_reseveKey;
}

void UIChatUserInfo::UsePhoneMsg(QString val)
{
    m_usePhoneMsg = val;
}

QString UIChatUserInfo::UsePhoneMsg() const
{
    return m_usePhoneMsg;
}

void UIChatUserInfo::UserId(QString val)
{
    m_userId = val;
}

QString UIChatUserInfo::UserId() const
{
    return m_userId;
}

void UIChatUserInfo::UserType(EUSERTYPE val)
{
    m_userType = val;
}

EUSERTYPE UIChatUserInfo::UserType() const
{
    return m_userType;
}

void UIChatUserInfo::StaticId(QString val)
{
    m_viewUserId = val;
}

QString UIChatUserInfo::StaticId() const
{
    return m_viewUserId;
}

void UIChatUserInfo::IsWatcher(bool val)
{
    m_isWatcher = val;
}

bool UIChatUserInfo::IsWatcher() const
{
    return m_isWatcher;
}

void UIChatUserInfo::OtherInfos(const CommInfo& val)
{
    if (m_otherInfos == nullptr)
    {
        m_otherInfos = new CommInfo(val);
    }
    else
    {
        delete m_otherInfos;
        m_otherInfos = new CommInfo(val);
    }
}

CommInfo* UIChatUserInfo::OtherInfos() const
{
    return m_otherInfos;
}

QString UIChatUserInfo::GetDisplayName()
{

    QString ret = m_userId;

    if (m_userType == E_VISITOR)
    {

        //todo 9999
        /*
        if (m_cardInfo && !m_cardInfo->Name().isEmpty())
            return m_cardInfo->Name();
        if (!OtherInfos()->GetValueByKey(DOUYI_NICKNAME).isEmpty())
                    return OtherInfos()->GetValueByKey(DOUYI_NICKNAME);
                    */
        QString position = "";
        if (m_otherInfos)
        {
            position = m_otherInfos->GetValueByKey("visitor_location_city");
            if (position.isEmpty())
            {
                position = m_otherInfos->GetValueByKey("visitor_location_province");
            }
            else
            {
                position += m_otherInfos->GetValueByKey("visitor_location_district");
            }

            if (position.isEmpty())
            {
                position = m_otherInfos->GetValueByKey("visitor_location_country");
            }
        }
        ret = UtilsHelper::GetDefaultName(position, ret);
    }
    return ret;
}

QString UIChatUserInfo::GetVisitorLocation() const
{
    QString city     = "";
    QString province = "";
    QString country  = "";
    QString district = "";
    QString ip       = "";
    if (m_otherInfos)
    {
        district = m_otherInfos->GetValueByKey("visitor_location_district");
        city     = m_otherInfos->GetValueByKey("visitor_location_city");
        province = m_otherInfos->GetValueByKey("visitor_location_province");
        country  = m_otherInfos->GetValueByKey("visitor_location_country");
        country  = m_otherInfos->GetValueByKey("ip");
    }
    QString format = "%1%2%3【%4】";
    return format.arg(country).arg(province).arg(city).arg(ip);
}

UICardInfo* UIChatUserInfo::CardInfo() const
{
    return m_cardInfo;
}

void UIChatUserInfo::CardInfo(UICardInfo* cardInfo)
{
    if (m_cardInfo)
        delete m_cardInfo;
    m_cardInfo = new UICardInfo(*cardInfo);
}


QStringList UIChatUserInfo::chatidlist() const
{
    return m_chatidlist;
}

void UIChatUserInfo::addchatidlist(QString charid)
{
    m_chatidlist.append(charid);
    m_chatidlist = m_chatidlist.toSet().toList();
}

void UIChatUserInfo::addchatidlist(QStringList charids)
{
    m_chatidlist.append(charids);
    m_chatidlist = m_chatidlist.toSet().toList();
}

void UIChatUserInfo::setchatidlist(QStringList list)
{
    m_chatidlist = list;
}
