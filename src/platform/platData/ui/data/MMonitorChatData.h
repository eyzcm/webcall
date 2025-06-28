#pragma once
#include "utils/Plat_EnumDefine.h"
#include "data/MBaseData.h"
#include "data/UIChatInfo.h"
#include <QMap>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT MMonitorChatData : public MBaseData
{
    Q_OBJECT
public:
    MMonitorChatData(void);
    MMonitorChatData(const MMonitorChatData& o);
    ~MMonitorChatData(void);
    void InitChatInfo(const UIChatInfo& info,QString name,QString departmentId, QString departmentName);
    bool IsFilter(QString searchKey, CommInfo filterInfo);
    void CloneData(const MMonitorChatData& o);

public:
    QString Name() const;
    void    Name(QString val);
    QString Vid() const;
    void    Vid(QString val);
    QString Userid() const;
    void    Userid(QString val);
    QString ChatId() const;
    void    ChatId(QString val);
    QString CustomerId() const;
    void    CustomerId(QString val);
    QString CustomerName() const;
    void    CustomerName(QString val);

    QString GetUIChatInfoValueByKey(QString strKey) const;
    void    SetUIChatInfoValueByKey(QString strKey, QString strVal);

    QString SearchingType() const;
    void    SearchingType(QString val);

    QString Awake() const;
    void    Awake(QString val);

    QString AwakeInfo() const;
    void    AwakeInfo(QString val);

    QString NickName() const;
    void    NickName(QString val);

    QDateTime ChatCloseTime() const;
    void      ChatCloseTime(QDateTime val);

    QMap<QString, QString> VisitorInfoMap() const;
    void                   VisitorInfoMap(QMap<QString, QString> val);

private:
    QString m_name;
    QString m_userid;
    QString m_vid;
    QString m_chatId;
    QString m_customerId;
    QString m_customerName;

    QString   m_strSearchingType; //平台类型: BCP百度；WeChatPublic微信公众号
    QString   m_strAwake;         //是否可唤醒 0不可唤醒；1可唤醒
    QString   m_strAwakeInfo;     //可唤醒相关参数
    QString   m_strNickName;
    QDateTime m_chatCloseTime; //对话结束时间

    QMap<QString, QString> m_visitorInfoMap;
};
