#include "AutoMonitor.h"

AutoMonitor::AutoMonitor(QString chatId) : TcpCmd()
{
    m_chatId = chatId;
}

AutoMonitor::~AutoMonitor()
{
}

void AutoMonitor::MakeCmd()
{
    Uri(QString("/im/message").arg(m_chatId));
    Body("");
}
