#pragma once

#include "utils/Plat_EnumDefine.h"
#include "UIChatUserInfo.h"
#include "data/CommInfo.h"

#include <QDateTime>
#include <QList>
#include <QMetaType>
#include <QObject>
#include "platdata_global.h"


class  PLATDTA_EXPORT UIChatInfo : public QObject
{
    Q_OBJECT
public:
    explicit UIChatInfo(QObject * parent = nullptr);
    UIChatInfo(const UIChatInfo& info);
    UIChatInfo(UIChatInfo* info);
    ~UIChatInfo(void);
     UIChatInfo  & operator=(const UIChatInfo & info){
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
        return  *this;
    }

public:
    QString ChatId() const;
    void    ChatId(QString val);

    QString CustomerID() const;
    void    CustomerID(QString val);

    QString TokeOverID() const;
    void    TokeOverID(QString val);

    EMESSAGETYPE MsgType() const
    {
        return m_type;
    }
    void MsgType(EMESSAGETYPE val)
    {
        m_type = val;
    }

    QString CustomerName() const;
    void    CustomerName(QString val);

    QString CreateTime() const;
    void    CreateTime(QString val);

    QString UserMsgCount() const;
    void    UserMsgCount(QString val);

    QString VisitorMsgCount() const;
    void    VisitorMsgCount(QString val);

    QString ChatUrl() const;
    void    ChatUrl(QString val);

    QString Visitor_location_province() const;
    void    Visitor_location_province(QString val);

    QString Visitor_location_city() const;
    void    Visitor_location_city(QString val);

    QString Visitor_location_district() const;
    void    Visitor_location_district(QString val);

    QString Visitor_location_country() const;
    void    Visitor_location_country(QString val);

    QString Visitor_ip() const;
    void    Visitor_ip(QString val);

    QString Visitor_ipv6() const;
    void    Visitor_ipv6(QString val);

    QString Chat_type() const;
    void    Chat_type(QString val);

    QString SiteId() const;
    void    SiteId(QString val);

    bool TempChat() const;
    void TempChat(bool val);

    QString TransferUserId() const;
    void    TransferUserId(QString val);

    QString PreCustomerId() const;
    void    PreCustomerId(QString val);

    QList<UIChatUserInfo*> ChatUserList() const
    {
        return m_uiChatUserList;
    }
    void ChatUserList(QList<UIChatUserInfo*> val)
    {
        m_uiChatUserList = val;
    }

    void AddChatUser(UIChatUserInfo* userinfo);

    bool HasVisitorByVid(const QString& vid);

    bool HasVisitorByUserId(const QString& userid);

    UIChatUserInfo* GetChatUserInfoByVid(const QString& vid) const;

    UIChatUserInfo* GetChatUserInfoByUserId(const QString& userid) const;
    UIChatUserInfo* GetVisitorInfo() const;
    UIChatUserInfo* GetCustomerInfo() const;
    QString         GetDisplayName() const;

    bool IsWatch(QString userid) const;

    int AiChatFlag() const
    {
        return m_aiChatFlag;
    }
    void AiChatFlag(int val)
    {
        m_aiChatFlag = val;
    }
    bool Flash() const
    {
        return m_isFlash;
    }
    void Flash(bool val)
    {
        m_isFlash = val;
    }
    bool NewChat() const
    {
        return m_isNewChat;
    }
    void NewChat(bool val)
    {
        m_isNewChat = val;
    }

    bool IsAckLeave() const
    {
        return m_isAckLeave;
    }
    void IsAckLeave(bool val)
    {
        m_isAckLeave = val;
    }

    int TakeoverFlag() const
    {
        return m_takeoverFlag;
    }
    void TakeoverFlag(int val)
    {
        m_takeoverFlag = val;
    }

    int InviteMode() const
    {
        return m_inviteMode;
    }
    void InviteMode(int val)
    {
        m_inviteMode = val;
    }

    int BatchId() const
    {
        return m_batchId;
    }
    void BatchId(int val)
    {
        m_batchId = val;
    }

    QMap<QString, QString> GetExts() const;

    void SetExts(QMap<QString, QString> map);
    void AddExts(QString key, QString val);

    QString GetKeyword();

    QString SummaryTag() const;
    void    SummaryTag(QString val);

    QString ChatAwaken() const;
    void    ChatAwaken(QString val);

    QString SearchingType() const;
    void    SearchingType(QString val);

    QString EffectiveAllocate() const;
    void    EffectiveAllocate(QString val);

    QString Awake() const;

    void Awake(QString val);

    QString AwakeInfo() const;
    void    AwakeInfo(QString val);

    QString NickName() const;
    void    NickName(QString val);

//todo 需求5332/10855 敏感词屏蔽
    QString SensitiveWords() const;
    void    SensitiveWords(QString val);

    QDateTime ChatCloseTime() const;
    void      ChatCloseTime(QDateTime val);

    UICardInfo * CardInfo();

    QString Ssid() const
    {
        return m_ssid;
    }
    void Ssid(QString val)
    {
        m_ssid = val;
    }



private:
    QString      m_ssid;
    QString m_chatId;
    QString m_customerID;
    QString m_tokeoverID;    //接管人
    QString m_preTakeoverID; //

    QString m_customerName;

    EMESSAGETYPE m_type;
    QString      m_createTime;
    QString      m_userMsgCount;
    QString      m_visitorMsgCount;
    QString      m_chatUrl;
    QString      m_visitor_location_province;
    QString      m_visitor_location_city;
    QString      m_visitor_location_district;
    QString      m_visitor_location_country;
    QString      m_visitor_ip;
    QString      m_visitor_ipv6;
    QString      m_chat_type;
    QString      m_siteId;
    QString      m_ocpcData;

    QString m_preCustomerId;
    bool    m_isTempChat;
    bool    m_isNewChat;
    bool    m_isFlash;
    int     m_aiChatFlag; // 0表示 人工客服的对话， 1表示机器人对话 2 表示人工参与对话  3 表示人工接管对话
    int     m_batchId;    //用于处理对话同步问题
    bool    m_isAckLeave;
    int     m_takeoverFlag;

    //邀请模式
    int m_inviteMode;

    //
    QString m_effectiveAllocate;

    QString m_strSummaryTag; //名片标签
    QString m_strChatAwaken; //对话唤醒

    QString   m_strSearchingType; //平台类型: BCP百度；WeChatPublic微信公众号
    QString   m_strAwake;         //是否可唤醒 0不可唤醒；1可唤醒
    QString   m_strAwakeInfo;     //可唤醒相关参数
    QString   m_strNickName;
    QDateTime m_chatCloseTime; //对话结束时间
    //todo 需求5332/10855
    QString   m_sensitiveWords; //敏感词屏蔽

    QString                m_transferUserId;
    QList<UIChatUserInfo*> m_uiChatUserList;
    QMap<QString, QString> m_extsmap;
};
Q_DECLARE_METATYPE(UIChatInfo)
