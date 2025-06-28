#include "ConversationCmd.h"

ConversationCmd::ConversationCmd(void) : TcpCmd()
{
}

ConversationCmd::~ConversationCmd(void)
{
}

void ConversationCmd::MakeCmd()
{
    QString strFormat = "/im/control?userId=%1&type=%2&viewId=%3";
    Uri(strFormat.arg(UserId()).arg(ConversationsType()).arg(this->ViewId()));
    Body(this->ViewId());
}
