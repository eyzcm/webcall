#include "UpdateLastMessageTime.h"

UpdateLastMessageTime::UpdateLastMessageTime(QString chatId) : TcpCmd()
{
    m_chatId = chatId;
}

UpdateLastMessageTime::~UpdateLastMessageTime()
{
}

void UpdateLastMessageTime::MakeCmd()
{
    Uri(QString("/im/updateLastMessageTime?chatId=%1").arg(m_chatId));
    Body("");
}
