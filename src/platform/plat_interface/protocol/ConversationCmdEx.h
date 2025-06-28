#pragma once
#include "protocol/tcpcmd.h"
//#include "visitormonitortcpcmd.h"
#include "data/CommInfo.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT ConversationCmdEx : public TcpCmd
{
    Q_OBJECT

public:
    ConversationCmdEx(void);
    ~ConversationCmdEx(void);

public:
    virtual void MakeCmd();

    CommInfo* ExInfo()
    {
        return &m_exInfo;
    };

public:
    CommInfo m_exInfo;

    Property_QString(UserId, userId);
    // viewId为访客本次标识
    Property_QString(ViewId, viewId);
    Property_QString(ConversationsType, conversationsType);
};
