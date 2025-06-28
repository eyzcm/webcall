#pragma once
#include "utils/Plat_EnumDefine.h"
#include "platdata_global.h"
//#include "data/UICardInfo.h"
#include <QDateTime>
#include <QObject>
#include "data/CommInfo.h"
#include "data/UICardInfo.h"

class PLATDTA_EXPORT UIVisitorInfo : public QObject
{
    Q_OBJECT
public:
    UIVisitorInfo(void);
    UIVisitorInfo(const UIVisitorInfo& info);
    UIVisitorInfo(UIVisitorInfo* pinfo);
    ~UIVisitorInfo(void);

    QString GetToolTip() const;

public:
    EVISITORSTATUS VisitorStatus() const;
    void           VisitorStatus(EVISITORSTATUS val);

    QString Vid() const;
    void    Vid(QString val);

    QString StaticId() const;
    void    StaticId(QString val);

    QString HeadImgUrl() const;
    void    HeadImgUrl(QString val);

    QString Gender() const;
    void    Gender(QString val);

    QString DisplayName() const;

    QString VisitorStartTime() const;
    void    VisitorStartTime(QString val);

    QString LocalOption() const;
    void    LocalOption(QString val);

    QString ChatUrl() const;
    void    ChatUrl(QString val);

    QString UrlAlias() const;
    void    UrlAlias(QString val);

    QString UrlTitle() const;
    void    UrlTitle(QString val);

    QString UrlRefer() const;
    void    UrlRefer(QString val);

    QString SearchEngineId() const;
    void    SearchEngineId(QString val);

    QString SearchEngineName() const;
    void    SearchEngineName(QString val);

    QString KeyWord() const;
    void    KeyWord(QString val);

    QString Extend_KeyWord() const;
    void    Extend_KeyWord(QString val);

    QString CustomerId() const;
    void    CustomerId(QString val);

    QString CustomerName() const;
    void    CustomerName(QString val);

    QString VisitorCount() const;
    void    VisitorCount(QString val);

    QString InviteCount() const;
    void    InviteCount(QString val);

    QString AcceptCount() const;
    void    AcceptCount(QString val);

    QString DenyCount() const;
    void    DenyCount(QString val);

    QString SiteId() const;
    void    SiteId(QString val);

    QString VisitorType() const;
    void    VisitorType(QString val);

    QString VisitorName() const;
    void    VisitorName(QString val);

    QString Visitor_location_province() const;
    void    Visitor_location_province(QString val);

    QString Visitor_location_district() const;
    void    Visitor_location_district(QString val);

    QString Visitor_location_city() const;
    void    Visitor_location_city(QString val);

    QString Visitor_location_country() const;
    void    Visitor_location_country(QString val);

    QString LastActiveUrl() const;
    void    LastActiveUrl(QString val);

    QString FirstActiveUrl() const;
    void    FirstActiveUrl(QString val);

    QString Ip() const;
    void    Ip(QString val);

    QString Ie() const;
    void    Ie(QString val);

    QString CreateTime() const;
    void    CreateTime(QString val);

    QString LastActiveUrlTitle() const;
    void    LastActiveUrlTitle(QString val);

    QString FirstActiveUrlTitile() const;
    void    FirstActiveUrlTitile(QString val);

    QString Resolution() const;
    void    Resolution(QString val);

    QString Os() const;
    void    Os(QString val);

    QString Lang() const;
    void    Lang(QString val);

    QString Chroma() const;
    void    Chroma(QString val);

    QString UserAgent() const;
    void    UserAgent(QString val);

    QString Device() const;
    void    Device(QString val);

    QString DeviceModel() const;
    void    DeviceModel(QString val);

    QString SpreadFlag() const
    {
        return m_spreadFlag;
    }
    void SpreadFlag(QString val)
    {
        m_spreadFlag = val;
    }

    QString PromotionId() const
    {
        return m_promotionId;
    }
    void PromotionId(QString val)
    {
        m_promotionId = val;
    }

    QString ViewPageCount() const
    {
        return m_viewPageCount;
    }
    void ViewPageCount(QString val)
    {
        m_viewPageCount = val;
    }

    bool IsHide() const
    {
        return m_isHide;
    }
    void IsHide(bool val)
    {
        m_isHide = val;
    }

    QString HasCard() const
    {
        return m_hasCard;
    }
    void HasCard(QString val)
    {
        m_hasCard = val;
    }

    UICardInfo * CardInfo() const
    {
        return m_cardInfo;
    }
    void CardInfo(UICardInfo* cardinfo)
    {
        m_cardInfo = cardinfo;
    }

    QString PromotionName() const
    {
        return m_promotionName;
    }
    void PromotionName(QString val)
    {
        m_promotionName = val;
    }

    QString SearchingType() const;
    void    SearchingType(QString val);

    QString Awake() const;
    void    Awake(QString val);

    QString AwakeInfo() const;
    void    AwakeInfo(QString val);

    QString NickName() const;
    void    NickName(QString val);

    QString PreCustomerId() const;
    void    PreCustomerId(QString val);

private:
    EVISITORSTATUS m_visitorStatus;
    QString        m_vid;
    QString        m_staticId;
    QString        m_visitorStartTime;
    QString        m_localOption;
    QString        m_chatUrl;
    QString        m_urlAlias;
    QString        m_urlTitle;
    QString        m_urlRefer;
    QString        m_searchEngineId;
    QString        m_searchEngineName;
    //搜索词
    QString m_keyWord;
    //关键词。目前只有百度有关键词。
    QString m_extend_keyWord;
    QString m_customerId;
    QString m_customerName;
    QString m_visitorCount;
    QString m_inviteCount;
    QString m_acceptCount;
    QString m_denyCount;
    QString m_siteId;
    QString m_visitorType;
    QString m_visitorName;

    QString m_visitor_location_province;
    QString m_visitor_location_city;
    QString m_visitor_location_district;
    QString m_visitor_location_country;

    QString m_lastActiveUrl;
    QString m_firstActiveUrl;
    QString m_ip;
    QString m_ie;
    QString m_createTime;
    QString m_lastActiveUrlTitle;
    QString m_firstActiveUrlTitile;

    QString m_resolution;
    QString m_os;
    QString m_device;
    QString m_deviceModel;
    QString m_spreadFlag;
    QString m_promotionId;
    QString m_promotionName;
    QString m_viewPageCount;

    QString m_lang;
    QString m_chroma;
    QString m_userAgent;

    QString m_hasCard;
    bool    m_isHide;

    QString m_strSearchingType; //平台类型: BCP百度；WeChatPublic微信公众号
    QString m_strAwake;         //是否可唤醒 0不可唤醒；1可唤醒
    QString m_strAwakeInfo;     //可唤醒相关参数
    QString m_strNickName;

    QString m_headImgUrl;
    QString m_gender;

    QString m_preCustomerId;

    UICardInfo* m_cardInfo;
};
