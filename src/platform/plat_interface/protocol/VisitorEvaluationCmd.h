#pragma once

#include "protocol/tcpcmd.h"
#include "plat_interface_global.h"

//访客评价
class PLAT_INTERFACE_EXPORT  VisitorEvaluationCmd : public TcpCmd
{
    Q_OBJECT

public:
    VisitorEvaluationCmd(void);
    ~VisitorEvaluationCmd(void);

    virtual void MakeCmd();

public:
    Property_QString(ChatId, chatId);
};
