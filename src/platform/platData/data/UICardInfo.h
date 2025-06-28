#pragma once

#include <QMap>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT UICardInfo : public QObject
{
public:
    UICardInfo(void);
    UICardInfo(const UICardInfo& info);
    ~UICardInfo(void);

public:
    QString UrlEncode(QString val) const;

    //QString ToString(QMap<QString,QString> map = QMap<QString,QString>()) const;
	QString ToString(QMap<QString, QString> map) const;
    QString BaseToString(QMap<QString,QString> map) const;

    QMap<QString, QString> ToMap() const;

    QString Area() const;
    void    Area(QString val);
    QString GroupId() const;
    void    GroupId(QString val);
    QString ChatURL() const;
    void    ChatURL(QString val);
    QString CompanyId() const;
    void    CompanyId(QString val);
    QString CompanyName() const;
    void    CompanyName(QString val);
    QString CreateTime() const;
    void    CreateTime(QString val);
    QString Email() const;
    void    Email(QString val);
    QString FirstURL() const;
    void    FirstURL(QString val);
    QString Id() const;
    void    Id(QString val);
    QString KeyWord() const;
    void    KeyWord(QString val);
    QString Mobile() const;
    void    Mobile(QString val);

    QString PhoneStatus() const;
    void    PhoneStatus(QString val);

    QString Msn() const;
    void    Msn(QString val);
    QString Name() const;
    void    Name(QString val);
    QString Note() const;
    void    Note(QString val);
    QString PromotionId() const;
    void    PromotionId(QString val);
    QString PromotionName() const;
    void    PromotionName(QString val);
    QString Qq() const;
    void    Qq(QString val);
    QString RepName() const;
    void    RepName(QString val);
    QString ReseveKey() const;
    void    ReseveKey(QString val);
    QString SearchEngine() const;
    void    SearchEngine(QString val);
    QString SearchHost() const;
    void    SearchHost(QString val);
    QString Sex() const;
    void    Sex(QString val);
    QString SpreadFlag() const;
    void    SpreadFlag(QString val);
    QString Ssid() const
    {
        return m_ssid;
    }
    void Ssid(QString val)
    {
        m_ssid = val;
    }

    QString Tel() const;
    void    Tel(QString val);
    QString Url() const;
    void    Url(QString val);
    QString UserId() const;
    void    UserId(QString val);
    QString VisitorStaticId() const;
    void    VisitorStaticId(QString val);
    QString ExtColumn1() const;
    void    ExtColumn1(QString val);
    QString ExtColumn10() const;
    void    ExtColumn10(QString val);
    QString ExtColumn2() const;
    void    ExtColumn2(QString val);
    QString ExtColumn3() const;
    void    ExtColumn3(QString val);
    QString ExtColumn4() const;
    void    ExtColumn4(QString val);
    QString ExtColumn5() const;
    void    ExtColumn5(QString val);
    QString ExtColumn6() const;
    void    ExtColumn6(QString val);
    QString ExtColumn7() const;
    void    ExtColumn7(QString val);
    QString ExtColumn8() const;
    void    ExtColumn8(QString val);
    QString ExtColumn9() const;
    void    ExtColumn9(QString val);

    QString ExtColumn11() const;
    void    ExtColumn11(QString val);
    QString ExtColumn12() const;
    void    ExtColumn12(QString val);
    QString ExtColumn13() const;
    void    ExtColumn13(QString val);
    QString ExtColumn14() const;
    void    ExtColumn14(QString val);
    QString ExtColumn15() const;
    void    ExtColumn15(QString val);

    QString Ip() const;
    void    Ip(QString val);
    QString Ipv6() const;
    void    Ipv6(QString val);

    QString CreateUserId() const;
    void    CreateUserId(QString val);
    QString EditTime() const;
    void    EditTime(QString val);
    QString EditUserId() const;
    void    EditUserId(QString val);
    QString GetValueByKey(QString strKey) const;
    void    SetValueByKey(QString strKey, QString strVal);

    QString Refer() const;
    void    Refer(QString val);

    QString ChatId() const;
    void    ChatId(QString val);

    QString Tag() const
    {
        return m_nTag;
    }
    void Tag(QString val)
    {
        m_nTag = val;
    }

    QString ChatType() const;
    void    ChatType(QString val);

    QString VisitorLocationCountry() const;
    void    VisitorLocationCountry(QString val);

    QString VisitorLocationProvince() const;
    void    VisitorLocationProvince(QString val);

    QString VisitorLocationCity() const;
    void    VisitorLocationCity(QString val);

    QString VisitorLocationDistrict() const;
    void    VisitorLocationDistrict(QString val);

    QString SiteId() const;
    void    SiteId(QString val);

    bool IsHasContact();
    bool IsAiEdit();

    QString LastLandingPage() const;
    void    LastLandingPage(QString val);

    QString BcpBiddingWord() const;
    void    BcpBiddingWord(QString val);

    QString ExtendKeyword() const;
    void    ExtendKeyword(QString val);

private:
    QString m_id;
    QString m_visitorStaticId;

    QString m_name;
    QString m_companyName;
    QString m_area;
    QString m_qq;
    QString m_mobile;
    QString m_phoneStatus;
    QString m_msn;
    QString m_note;
    QString m_tel;
    QString m_sex;
    QString m_email;
    QString m_nTag;
    QString m_ip;
    QString m_ipv6;
    QString m_extColumn1;
    QString m_extColumn10;
    QString m_extColumn2;
    QString m_extColumn3;
    QString m_extColumn4;
    QString m_extColumn5;
    QString m_extColumn6;
    QString m_extColumn7;
    QString m_extColumn8;
    QString m_extColumn9;
    QString m_extColumn11;
    QString m_extColumn12;
    QString m_extColumn13;
    QString m_extColumn14;
    QString m_extColumn15;

    QString m_companyId;

    QString m_keyWord;
    //对话页面
    QString m_chatURL;
    //首次访问网页URL
    QString m_firstURL;
    //来源页
    QString m_refer;
    //对话id
    QString m_chatId;
    QString m_visitor_location_country;
    QString m_visitor_location_province;
    QString m_visitor_location_city;
    QString m_visitor_location_district;
    //子站点
    QString m_siteId;
    //传chat_type， 对应PC， PHONE, WEIXIN, BCP等， 可从对话扩展信息中获取｛chat_type｝
    QString m_chat_type;
    //渠道ID
    QString m_promotionId;

    //渠道名称
    QString m_promotionName;



    //客户端回传后台给的数据
    QString m_createTime;
    QString m_createUserId;
    QString m_editTime;
    QString m_editUserId;

    //推广标识
    QString m_spreadFlag;
    // 扩展字段，对话的扩展字段key
    QString m_reseveKey;
    //搜索引擎
    QString m_searchEngine;
    //搜索服务器域名
    QString m_searchHost;

    //所属者，保存时不传该值
    QString m_userId;

    //暂时不清楚含义
    QString m_url;
    QString m_repName;
    QString m_ocpcData;
    QString m_lastLandingPage;
    QString m_groupId;

    //百度渠道 ssid
    QString m_ssid;

    QString m_bcpBiddingWord;
    QString m_extendKeyword;
};
