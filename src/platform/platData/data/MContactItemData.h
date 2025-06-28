#pragma once
#include "utils/Plat_EnumDefine.h"
#include "data/MBaseData.h"
#include <QMap>
#include <QObject>
#include "platdata_global.h"

class PLATDTA_EXPORT MContactItemData : public MBaseData
{
    Q_OBJECT

public:
    MContactItemData(void);
    MContactItemData(const MContactItemData& o);
    ~MContactItemData(void);

public:
    void InitData(QString strUUId, QString vid, QString userid, QString chatid, QString name, QString groupid, QString departmentId, EUINODEMODELTYPE etype);

signals:
    void S_OnNameChanged(QString strOldName, QString strNewName);

public:
    QString Name() const;
    void    Name(QString val);

    QString GetUUid() const;
    void    SetUUid(QString val);

    EUINODEMODELTYPE NoteType() const;
    void             NoteType(EUINODEMODELTYPE val);

    QString Groupid() const;
    void    Groupid(QString val);

    QString Userid() const;
    void    Userid(QString val);

    QString Vid() const;
    void    Vid(QString val);

    QString Chatid() const;
    void    Chatid(QString val);

    QString DepartmentId() const;
    void    DepartmentId(QString val);

    int  InviteMode() const;
    void InviteMode(int val);

    QString SearchingType() const;
    void    SearchingType(QString val);

    QString Awake() const;
    void    Awake(QString val);

    QString AwakeInfo() const;
    void    AwakeInfo(QString val);

    QString NickName() const;
    void    NickName(QString val);

private:
    QString          m_name;
    EUINODEMODELTYPE m_noteType;
    QString          m_groupid;
    QString          m_departmentId;
    QString          m_userid;
    QString          m_vid;
    QString          m_treeuuid;
    QString          m_chatid;
    int              m_inviteMode;

    QString m_strSearchingType; //平台类型: BCP百度；WeChatPublic微信公众号
    QString m_strAwake;         //是否可唤醒 0不可唤醒；1可唤醒
    QString m_strAwakeInfo;     //可唤醒相关参数
    QString m_strNickName;
};
