#pragma once
#include "utils/Plat_EnumDefine.h"
#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT CommChatMessageCmd : public TcpCmd
{

    Q_OBJECT

public:
    CommChatMessageCmd(void);
    ~CommChatMessageCmd(void);

    virtual void MakeCmd();

public:
    Property_QString(ChatId, chatId);
    Property_QString(VisitorId, vid);

public:
    ECHATACTION ChatAction() const
    {
        return m_eChatAction;
    }
    void ChatAction(ECHATACTION val)
    {
        m_eChatAction = val;
    }

private:
    ECHATACTION m_eChatAction;
};
