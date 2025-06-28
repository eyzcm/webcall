#include "TakeoverCmd.h"

TakeoverCmd::TakeoverCmd(QString chatId) : TcpCmd()
{
    m_chatId = chatId;
}

TakeoverCmd::~TakeoverCmd()
{
}

void TakeoverCmd::MakeCmd()
{
    Uri(QString("/im/control?chatId=%1&type=EVENT_ACK_RESPONSE").arg(m_chatId));
    Body("");
}
