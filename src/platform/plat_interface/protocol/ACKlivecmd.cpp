#include "ACKLiveCmd.h"

ACKLiveCmd::ACKLiveCmd(QString chatId) : TcpCmd()
{
    m_chatId = chatId;
}

ACKLiveCmd::~ACKLiveCmd()
{
}

void ACKLiveCmd::MakeCmd()
{
    Uri(QString("/im/control?chatId=%1&type=EVENT_ACK_RESPONSE").arg(m_chatId));
    Body("");
}
