#ifndef ICOMMONSIGNALINTERFACE_H
#define ICOMMONSIGNALINTERFACE_H

#include <QObject>
#include "utils/Plat_EnumDefine.h"

class ICommonSignalInterface
{
public:
    ICommonSignalInterface();

    virtual void DoTakeoverStatus(QString chatId, CHAT_TAKEOVER_FLAG flag) = 0;
    virtual void DoSaveHistoryMessage(QString body, QString chatid, QString from, EMESSAGETYPE type) = 0;
    virtual void DoAutoJoinTagBySendMsg(QString chatId) = 0;

};

#endif // ICOMMONSIGNALINTERFACE_H
