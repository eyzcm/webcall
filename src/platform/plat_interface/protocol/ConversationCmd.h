#pragma once
#include "protocol/tcpcmd.h"
//#include "visitormonitortcpcmd.h"
#include "plat_interface_global.h"

class PLAT_INTERFACE_EXPORT ConversationCmd : public TcpCmd
{
    Q_OBJECT

public:
    ConversationCmd(void);
    ~ConversationCmd(void);

public:
    virtual void MakeCmd();

public:
    Property_QString(UserId, userId);
    // viewId为访客本次标识
    Property_QString(ViewId, viewId);
    Property_QString(ConversationsType, conversationsType);
};
