#ifndef SENDMSGCMD_H
#define SENDMSGCMD_H

#include "utils/Plat_EnumDefine.h"
#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class  PLAT_INTERFACE_EXPORT SendMsgCmd : public TcpCmd
{

    Q_OBJECT

public:
    SendMsgCmd();
    ~SendMsgCmd();

    virtual void MakeCmd();

public:
    // to为目标客服
    Property_QString(MsgTo, msgTo);
    Property_QString(FileName, fileName);

    Property_QString(Robot, robot);
    Property_QString(RobotLogId, robotLogId);

    Property_QString(RecorderId, recorderId);

    Property_QString(ChatId, chatId);
    Property_Int(CallbackId, callbakId);

public:
    EMESSAGETYPE MsgType() const
    {
        return m_eMsgType;
    }
    void MsgType(EMESSAGETYPE val)
    {
        m_eMsgType = val;
    }

    EMESSAGESUBTYPE MsgSubtype() const
    {
        return m_eMsgSubtype;
    }
    void MsgSubtype(EMESSAGESUBTYPE val)
    {
        m_eMsgSubtype = val;
    }

    EUSERTYPE UserType() const
    {
        return m_eUserType;
    }
    void UserType(EUSERTYPE val)
    {
        m_eUserType = val;
    }

    bool RichText() const
    {
        return m_isRichText;
    }
    void RichText(bool val)
    {
        m_isRichText = val;
    }

    void ExtMap(QMap<QString, QString> map);

private:
    EMESSAGETYPE    m_eMsgType;
    EUSERTYPE       m_eUserType;
    EMESSAGESUBTYPE m_eMsgSubtype;
    bool            m_isRichText;

    QMap<QString, QString> m_map;
};

#endif // SENDMSGCMD_H
