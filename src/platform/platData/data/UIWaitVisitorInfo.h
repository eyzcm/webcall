#pragma once


#include "platdata_global.h"
#include <QDateTime>
#include <QObject>
#include "data/UICardInfo.h"

class PLATDTA_EXPORT UIWaitVisitorInfo : public QObject
{
    Q_OBJECT
public:
    UIWaitVisitorInfo(void);
    UIWaitVisitorInfo(const UIWaitVisitorInfo& info);
    UIWaitVisitorInfo(UIWaitVisitorInfo* info);
    ~UIWaitVisitorInfo(void);

public:
    QString UserId() const;
    void    UserId(QString val);

    QString Visitor_location_province() const;
    void    Visitor_location_province(QString val);

    QString Visitor_location_city() const;
    void    Visitor_location_city(QString val);

    QString Visitor_location_district() const;
    void    Visitor_location_district(QString val);

    QString Visitor_location_country() const;
    void    Visitor_location_country(QString val);

    QString GroupId() const;
    void    GroupId(QString val);

    QString Visitor_ip() const;
    void    Visitor_ip(QString val);

    QString Visitor_ipv6() const;
    void    Visitor_ipv6(QString val);

    QString Chat_type() const;
    void    Chat_type(QString val);

    QString JoinQueueTime() const;
    void    JoinQueueTime(QString val);

    QString ChatUrl() const;
    void    ChatUrl(QString val);

    QString SiteId() const;
    void    SiteId(QString val);

    QString Queue_index() const;
    void    Queue_index(QString val);

    QString PromotionId() const;
    void    PromotionId(QString val);

    QString PromotionName() const;
    void    PromotionName(QString val);


    QString ViewVid() const;
    void    ViewVid(QString val);

    QString DisplayName() const;

    QString GetVisitorLocation() const;

    QString KeyWord() const
    {
        return m_keyWord;
    }
    void KeyWord(QString val)
    {
        m_keyWord = val;
    }

    QString Refer() const
    {
        return m_refer;
    }
    void Refer(QString val)
    {
        m_refer = val;
    }

    QString FirstUrl() const
    {
        return m_firstUrl;
    }
    void FirstUrl(QString val)
    {
        m_firstUrl = val;
    }

    QString HasCard() const
    {
        return m_hasCard;
    }
    void HasCard(QString val)
    {
        m_hasCard = val;
    }

    QString Resolution() const;
    void    Resolution(QString val);

    QString Ie() const;
    void    Ie(QString val);

    QString Os() const;
    void    Os(QString val);

    QString Lang() const;
    void    Lang(QString val);

    QString Chroma() const;
    void    Chroma(QString val);

    QString UserAgent() const;
    void    UserAgent(QString val);

    QString DeviceModel() const;
    void    DeviceModel(QString val);

    QString     Device() const;
    void        Device(QString val);
    UICardInfo * CardInfo() const
    {
        return m_cardInfo;
    }
    void CardInfo(UICardInfo* cardinfo)
    {
        m_cardInfo = cardinfo;
    }

private:
    QString     userId;
    QString     viewVid;
    QString     m_visitor_location_province;
    QString     m_visitor_location_city;
    QString     m_visitor_location_district;
    QString     m_visitor_location_country;
    QString     m_groupId;
    QString     m_visitor_ip;
    QString     m_visitor_ipv6;
    QString     m_chat_type;
    QString     m_joinQueueTime;
    QString     m_chatUrl;
    QString     m_siteId;
    QString     m_queue_index;
    QString     m_promotionId;
    QString     m_promotionName;
    QString     m_keyWord;
    QString     m_refer;
    QString     m_firstUrl;
    QString     m_hasCard;
    QString     m_ie;
    QString     m_resolution;
    QString     m_os;
    QString     m_device;
    QString     m_deviceModel;
    QString     m_lang;
    QString     m_chroma;
    QString     m_userAgent;
    UICardInfo* m_cardInfo;
};
