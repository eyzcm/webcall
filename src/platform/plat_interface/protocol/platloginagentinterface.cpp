#include "platloginagentinterface.h"

PlatLoginAgentInterface::PlatLoginAgentInterface()
{

}



void PlatLoginAgentInterface::DoHttpReqLog(QString requrl, QString body)
{
    Q_EMIT S_HttpReqLog(requrl, body);
}
void PlatLoginAgentInterface::DoHttpResultFinishedLog(QString code, QString reason, QString body)
{
    Q_EMIT S_HttpResultFinishedLog(code, reason, body);
}

void PlatLoginAgentInterface::DoTcpReqLog(QString msgtype, QString body)
{
    Q_EMIT S_TcpReqLog(msgtype, body);
}

void PlatLoginAgentInterface::DoTcpResultLog(QString msgtype, QString body)
{
    Q_EMIT S_TcpResultLog(msgtype, body);
}
