#include "ConversationCmdEx.h"

ConversationCmdEx::ConversationCmdEx(void) : TcpCmd()
{
}

ConversationCmdEx::~ConversationCmdEx(void)
{
}

void ConversationCmdEx::MakeCmd()
{
    QString                strFormat = "/im/control?userId=%1&type=%2&viewId=%3";
    QString                reqtext   = strFormat.arg(UserId()).arg(ConversationsType()).arg(this->ViewId());
    QMap<QString, QString> infomap   = m_exInfo.InfoMap();

    for (auto iter = infomap.begin(); iter != infomap.end(); iter++)
    {
        reqtext += "&";
        reqtext += iter.key();
        reqtext += "=";
        reqtext += iter.value();
    }
    Uri(reqtext);
    Body(this->ViewId());
}
