#pragma once
#include "utils/Plat_EnumDefine.h"
#include "data/UICardInfo.h"
#include "data/CommInfo.h"
#include "platdata_global.h"
#include <QObject>
#include <QSet>

class PLATDTA_EXPORT UIChatUserInfo : public QObject
{
    Q_OBJECT

public:
    UIChatUserInfo(void);
    UIChatUserInfo(const UIChatUserInfo& info);
    ~UIChatUserInfo(void);


private:
    QString m_autoAccept;
    bool    m_isWatcher;
    QString m_reseveKey;
    QString m_usePhoneMsg;
    //访客的vid
    QString m_userId;
    //当前访客的vid对应的多个chatid,该字段用于处理,拉取当前通话中访客接口时,传入的chatids字段
    QStringList m_chatidlist;

    //访客的静态id
    QString   m_viewUserId;
    EUSERTYPE m_userType;
    QString   m_watcher;
    CommInfo* m_otherInfos;

    UICardInfo * m_cardInfo;

public:
    QString AutoAccept() const;
    void    AutoAccept(QString val);

    QString Watcher() const;
    void    Watcher(QString val);

    QString ReseveKey() const;
    void    ReseveKey(QString val);

    QString UsePhoneMsg() const;
    void    UsePhoneMsg(QString val);

    QString UserId() const;
    void    UserId(QString val);

    EUSERTYPE UserType() const;
    void      UserType(EUSERTYPE val);

    QString StaticId() const;
    void    StaticId(QString val);

    bool IsWatcher() const;
    void IsWatcher(bool val);

    CommInfo* OtherInfos() const;
    void      OtherInfos(const CommInfo& val);

    QString GetDisplayName();

    QString GetVisitorLocation() const;

    UICardInfo* CardInfo() const;
    void        CardInfo(UICardInfo* cardInfo);


    QStringList chatidlist() const;
    void        addchatidlist(QString charid);
    void        addchatidlist(QStringList charids);
    void        setchatidlist(QStringList list);
};
