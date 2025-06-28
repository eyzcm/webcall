#pragma once
#include "utils/Plat_EnumDefine.h"
#include "data/MBaseData.h"
#include "utils/DefineHelper.h"
#include "MMonitorChatData.h"
#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include "platdata_global.h"

class PLATDTA_EXPORT MChatItemData : public MBaseData
{
    Q_OBJECT
public:
    MChatItemData(void);
    MChatItemData(const MChatItemData& o);

    ~MChatItemData(void);

public:
    void UpdataData(const MChatItemData& o);
    void InitData(QString vid, QString userid, QString chatid, QString name, EUSERTYPE eUserType, ECHATITEMSTATE eState);
    void InitData(MMonitorChatData data);

    bool IsFindVisitorByKey(QString key, QString type);
    bool IsFindCustomerByKey(QString key);

signals:
    void S_OnNameChanged(QString strOldName, QString strNewName);
    void S_OnChatStateChanged(ECHATITEMSTATE strOldState, ECHATITEMSTATE strNewState);

public:
    QString Name() const;
    void    Name(QString val);

    ECHATITEMSTATE ChatState() const;
    void           ChatState(ECHATITEMSTATE val);

    EUSERTYPE UserType() const;
    void      UserType(EUSERTYPE val);

    QString Userid() const;
    void    Userid(QString val);

    QString Vid() const;
    void    Vid(QString val);

    QString Chatid() const;
    void    Chatid(QString val);

    bool TakeOver() const;
    void TakeOver(bool val);

    bool NewChat() const;
    void NewChat(bool val);

    QString EffectiveAllocate() const;
    void    EffectiveAllocate(QString val);

    int  InviteMode() const;
    void InviteMode(int val);

    int  AiCreate() const;
    void AiCreate(int val);

    int  UpdateCardType() const;
    void UpdateCardType(int val);

    QString SearchingType() const;
    void    SearchingType(QString val);

    QString Awake() const;
    void    Awake(QString val);

    QString AwakeInfo() const;
    void    AwakeInfo(QString val);

    QString NickName() const;
    void    NickName(QString val);

    void RemoveReserveRobot(int val);
    int  RemoveReserveRobot() const;

    QDateTime ChatCloseTime() const;
    void      ChatCloseTime(QDateTime val);

    EMESSAGETYPE MsgType() const
    {
        return m_msgtype;
    }
    void MsgType(EMESSAGETYPE val)
    {
        m_msgtype = val;
    }
    Property_QString(Summary_type, summary_type);       //对话总结标签
    Property_QString(Summary_explain, summary_explain); // 对话总结标签说明

protected:
    QString        m_name;
    ECHATITEMSTATE m_chatState;
    EUSERTYPE      m_userType;
    QString        m_userid;
    QString        m_vid;
    QString        m_chatid;
    //已接管
    bool m_isTakeOver;
    bool m_isNewChat;

    int m_inviteMode;
    int m_aiCreate;

    int m_updateCardType; // 1:切换节点  2:接收到访客消息

    QString      m_strSearchingType; //平台类型: BCP百度；WeChatPublic微信公众号
    QString      m_strAwake;         //是否可唤醒 0不可唤醒；1可唤醒
    QString      m_strAwakeInfo;     //可唤醒相关参数
    QString      m_strNickName;
    QDateTime    m_chatCloseTime; //对话结束时间
    QString      m_effectiveAllocate;
    int          m_removeReserveRobot;
    EMESSAGETYPE m_msgtype;
};
typedef QSharedPointer<MChatItemData> MChatItemDataPtr;

